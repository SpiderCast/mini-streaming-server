/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_fe.h"
#include "ixsys_print.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/dvb/version.h>
#include "ixstb_fe_dvbs.h"
#include "ixstb_fe_atsc.h"
#include "ix_list.h"
#include "ixsys_db_dvbs.h"

/* ****************************************************************************************************************** */

typedef struct _ixstb_fe_t {
    struct list_head    list;

#define DEVPATH_STRLEN  64
#define DEVNAME_STRLEN  128
#define DEVDESC_UNUSED  -1

    char        devpath[DEVPATH_STRLEN];
    char        devname[DEVNAME_STRLEN];
    fe_type_t   devtype;
    int         devdesc;
    int16_t     adapter;
    int16_t     frontend;

    int         rf_id;
} ixstb_fe_t;

/* ****************************************************************************************************************** */

static ixstb_fe_t   fe_list_;

/* ****************************************************************************************************************** */

static ixstb_fe_t* find_fe_use_id(int id)
{
    ixstb_fe_t  *pos;
    int         n = 0;

    list_for_each_entry(pos, &fe_list_.list, list) {
        if (id == n++) {
            return pos;
        }
    }

    return NULL;
}

static int try_lock_dvbs(ixstb_fe_t *p, int rf_id)
{
    ix_dvbs_t       dvbs;
    ix_dvbs_tp_t    tp;

    if (ixsys_get_dvbs_tp(DB_PATH_DATA, rf_id, &tp) == 0) {
        if (ixsys_get_dvbs(DB_PATH_DATA, tp.sat_id, &dvbs) == 0) {
            return ixstb_fe_dvbs_locking(p->devdesc, &dvbs, &tp);
        }
    }

    return -1;
}

static int try_lock_atsc(ixstb_fe_t *p, int rf_id)
{
    return ixstb_fe_atsc_locking(p->devdesc, rf_id);
}

static int find_fedev()
{
    struct dvb_frontend_info    fe_info;
    int     adapter, frontend, fd;
    char    str[DEVPATH_STRLEN];
    ixstb_fe_t  *fe;

    for (adapter = 0; adapter < 10; adapter++) {
        for (frontend = 0; frontend < 10; frontend++) {
            memset(str, 0, sizeof(str));
            snprintf(str, sizeof(str), "/dev/dvb/adapter%d/frontend%d", adapter, frontend);

            if ((fd = open(str, O_RDONLY)) >= 0) {
                if (ioctl(fd, FE_GET_INFO, &fe_info) != -1) {
                    fe = (ixstb_fe_t*)calloc(1, sizeof(ixstb_fe_t));

                    strcpy(fe->devpath, str);
                    strcpy(fe->devname, fe_info.name);
                    fe->devtype  = fe_info.type;
                    fe->devdesc  = DEVDESC_UNUSED;
                    fe->adapter  = adapter;
                    fe->frontend = frontend;
                    fe->rf_id    = DEVDESC_UNUSED;

                    list_add_tail(&fe->list, &fe_list_.list);
                }

                close(fd);
            }
        }
    }

    return 0;
}

int ixstb_fe_init()
{
#if DVB_API_VERSION < 5
# error DVBAPI should greater then 5
#endif

    INIT_LIST_HEAD(&fe_list_.list);
    return find_fedev();
}

int ixstb_fe_update()
{
    ixstb_fe_t  *pos;

    while (!list_empty(&fe_list_.list)) {
        pos = list_first_entry(&fe_list_.list, typeof(*pos), list);
        list_del(&pos->list);
        free(pos);
    }

    return find_fedev();
}

int ixstb_fe_get_path(int id, char *path, size_t len)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);

    if (pos) {
        strcpy(path, pos->devpath);
        return 0;
    }

    return -1;
}

int ixstb_fe_get_name(int id, char *name, size_t len)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);

    if (pos) {
        strcpy(name, pos->devname);
        return 0;
    }

    return -1;
}

int ixstb_fe_get_type(int id, fe_type_t *type)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);

    if (pos) {
        *type = pos->devtype;
        return 0;
    }

    return -1;
}

int ixstb_fe_get_desc(int id, int *fd)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);

    if ((pos) && (pos->devdesc != DEVDESC_UNUSED)) {
        *fd = pos->devdesc;
        return 0;
    }

    return -1;
}

int ixstb_fe_get_devid(int id, int16_t *adapter, int16_t *frontend)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);

    if (pos) {
        *adapter  = pos->adapter;
        *frontend = pos->frontend;
        return 0;
    }

    return -1;
}

int ixstb_fe_try_lock(int id, int rf_id)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);
    int         res = -1;

    if (!pos) {
        print_error("illegal frontend id: %d", id);
        return -1;
    }

    if (pos->devdesc == DEVDESC_UNUSED) {
        if ((pos->devdesc = open(pos->devpath, O_RDWR|O_NONBLOCK)) < 0) {
            print_error("frontend open error: %s", pos->devpath);
            return -1;
        }
    }

    switch (pos->devtype) {
    case FE_QPSK:
        res = try_lock_dvbs(pos, rf_id);
        break;
    case FE_QAM:
    case FE_OFDM:
    case FE_ATSC:
        res = try_lock_atsc(pos, rf_id);
        break;
    }

    pos->rf_id = rf_id;

    return res;
}

int ixstb_fe_unlock(int id)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);

    if (!pos) {
        print_error("illegal frontend id: %d", id);
        return -1;
    }

    if (pos->devdesc == DEVDESC_UNUSED) {
        print_warn("already closed: %s", pos->devpath);
        return 0;
    }

    switch (pos->devtype) {
    case FE_QPSK:
        ixstb_fe_dvbs_unlocking(pos->devdesc);
        break;
    case FE_QAM:
    case FE_OFDM:
    case FE_ATSC:
        break;
    }

    close(pos->devdesc);
    pos->devdesc = DEVDESC_UNUSED;
    pos->rf_id   = DEVDESC_UNUSED;


    return 0;
}

/* TeVii S660 에서 정확히 동작하지 않는 것으로 보임 - 사용 금지 */
int ixstb_fe_get_param(int id, struct dvb_frontend_parameters *param)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);
    // int         fd;

    if (!pos) {
        print_error("illegal frontend id: %d", id);
        return -1;
    }

    if (pos->devdesc == DEVDESC_UNUSED) {
        print_error("do not activated: %d", id);
        return -1;
    }

#if 0
    if ((fd = open(pos->devpath, O_RDONLY)) >= 0) {
        if (ioctl(fd, FE_GET_FRONTEND, param) >= 0) {
            close(fd);
            return 0;
        }

        close(fd);
    }
#else
    if (pos->rf_id == DEVDESC_UNUSED) {
        print_error("do not try the locking");
        return -1;
    }

    switch (pos->devtype) {
    default:
    case FE_QPSK:
        {
            ix_dvbs_tp_t    tp;

            ixsys_get_dvbs_tp(DB_PATH_DATA, pos->rf_id, &tp);

            param->frequency = tp.freq * 1000;
            param->u.qpsk.symbol_rate = tp.sym * 1000;
        }
        break;
    }
#endif

    return -1;
}

fe_status_t ixstb_fe_check_lock(int id, int timeout)
{
    fe_status_t status;
    ixstb_fe_t  *pos = find_fe_use_id(id);

    if (!pos) {
        print_error("illegal frontend id: %d", id);
        return 0;
    }

    if (pos->devdesc == DEVDESC_UNUSED) {
        print_error("do not activated: %d", id);
        return 0;
    }

    while (1) {
        if (ioctl(pos->devdesc, FE_READ_STATUS, &status) == -1) {
            print_error("frontend read status error");
            return 0;
        }

        if (status & FE_HAS_SIGNAL)
            break;

        if (timeout > 0) {
            usleep(1000 * 100);
            timeout -= 100;
        } else {
            break;
        }
    }

    return status;
}

fe_status_t ixstb_fe_get_signal_status(int id, int *ber, int *pwr, int *snr, int *unc)
{
    ixstb_fe_t  *pos = find_fe_use_id(id);
    fe_status_t status;
    uint32_t    local_ber, local_unc;
    uint16_t    local_strength, local_snr;

    if (!pos) {
        print_error("illegal frontend id: %d", id);
        return 0;
    }

    if (pos->devdesc == DEVDESC_UNUSED) {
        print_error("do not activated: %d", id);
        return 0;
    }

    if (ioctl(pos->devdesc, FE_READ_STATUS, &status) == -1) {
        print_error("read status error");
        return -1;
    }

    if (ber) {
        if (ioctl(pos->devdesc, FE_READ_BER, &local_ber) == -1) {
            print_error("read ber error");
            return -1;
        }

        *ber = local_ber;
    }

    if (pwr) {
        if (ioctl(pos->devdesc, FE_READ_SIGNAL_STRENGTH, &local_strength) == -1) {
            print_error("read signal_strength error");
            return -1;
        }

        *pwr = local_strength;
    }

    if (snr) {
        if (ioctl(pos->devdesc, FE_READ_SNR, &local_snr) == -1) {
            print_error("read snr error");
            return -1;
        }

        *snr = local_snr;
    }

    if (unc) {
        if (ioctl(pos->devdesc, FE_READ_UNCORRECTED_BLOCKS, &local_unc) == -1) {
            print_error("read uncorrected_blocks error");
            return -1;
        }

        *unc = local_unc;
    }

    print_debug("lock=> status(0x%x), BER(%d), PWR(%d), SNR(%d), UNC(%d)\n", status, local_ber, local_strength, local_snr, local_unc);

    return status;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
