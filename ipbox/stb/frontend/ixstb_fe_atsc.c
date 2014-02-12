/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_fe_atsc.h"
#include "ixsys_print.h"
#include <linux/dvb/frontend.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>

/* ****************************************************************************************************************** */

#define FE_PROPERTIES_LIMIT     10

/* ****************************************************************************************************************** */

static int set_tune(int fd, int rf_id)
{
    struct dvb_frontend_parameters  tuneto;
    struct dvb_frontend_event       ev;
    struct dtv_properties   dtv_cmd;
    struct dtv_property     clear = { .cmd = DTV_CLEAR };
    struct dtv_property     *props;
    int     num = 0;

    tuneto.frequency = rf_id * 1000 * 1000;
    tuneto.inversion = INVERSION_AUTO;
    tuneto.u.vsb.modulation = VSB_8;

    print_debug("tuneto.frequency: %d\n", tuneto.frequency);

    /* discard  stale QPSK events */
    while (ioctl(fd, FE_GET_EVENT, &ev) != -1) ;

    // if (ioctl(fd, FE_SET_FRONTEND, &tuneto) == -1) {
    //     print_error("set_frontend error");
    //     return -1;
    // }

    dtv_cmd.num = 1;
    dtv_cmd.props = &clear;

    if (ioctl(fd, FE_SET_PROPERTY, &dtv_cmd) == -1) {
        print_error("FE_SET_PROPERTY [clear] error");
        return -1;
    }

    props = (struct dtv_property*)calloc(FE_PROPERTIES_LIMIT, sizeof(struct dtv_property));

    props[num].cmd = DTV_DELIVERY_SYSTEM;
    props[num].u.data = SYS_ATSC;
    num++;
    props[num].cmd = DTV_MODULATION;
    props[num].u.data = tuneto.u.vsb.modulation;
    num++;
    props[num].cmd = DTV_FREQUENCY;
    props[num].u.data = tuneto.frequency;
    num++;
    props[num].cmd = DTV_TUNE;
    num++;

    dtv_cmd.num = num;
    dtv_cmd.props = props;

    if (ioctl(fd, FE_SET_PROPERTY, &dtv_cmd) == -1) {
        print_error("FE_SET_PROPERTY error: %s", strerror(errno));
        free(props);
        return -1;
    }

    free(props);

    return 0;
}

int ixstb_fe_atsc_locking(int fd, int rf_id)
{
    int     freq = 470;
    int     ch = 14;
    int     n;
    fe_status_t status;
    uint32_t    local_ber, local_unc;
    uint16_t    local_strength, local_snr;

    for (n = 0; n < 38; n++) {
        print_debug("try lock : CH(%d) FREQ(%d)\n", ch + n, freq + n * 6);

        set_tune(fd, freq + (n * 6));
        sleep(1);

        if (ioctl(fd, FE_READ_STATUS, &status) == -1) {
            print_error("read status error");
        }

        if (ioctl(fd, FE_READ_BER, &local_ber) == -1) {
            print_error("read ber error");
        }

        if (ioctl(fd, FE_READ_SIGNAL_STRENGTH, &local_strength) == -1) {
            print_error("read signal_strength error");
        }

        if (ioctl(fd, FE_READ_SNR, &local_snr) == -1) {
            print_error("read snr error");
        }

        if (ioctl(fd, FE_READ_UNCORRECTED_BLOCKS, &local_unc) == -1) {
            print_error("read uncorrected_blocks error");
        }

        print_debug("lock=> status(0x%x), BER(%d), PWR(%d), SNR(%d), UNC(%d)\n", status, local_ber, local_strength, local_snr, local_unc);
    }

    return 0;
}

int ixstb_fe_atsc_unlocking(int fd)
{
    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
