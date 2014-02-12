/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_fakepat.h"
#include "ixsys_print.h"
#include <libucsi/crc32.h>
#include "ix_list.h"

/* ****************************************************************************************************************** */

#define PAT_P_IN_TP(p)              (p + 5)
#define PAT_BONE_LEN                8

/* 4byte header + 1byte adaption field length : 5 */
#define MAKE_TP_HEADER(p)           memcpy(p, "\x47\x40\x00\x10\x00", 5)
/* table_id ~ last_section_number : 8 */
#define MAKE_PAT_BONE(p)            memcpy(p + 5, "\x00\xb0\x00\x00\x00\xc3\x00\x00", 8)

#define MAKE_PAT_SECLEN(p, len)     PAT_P_IN_TP(p)[1] |= (len >> 8) & 0x0f; PAT_P_IN_TP(p)[2] = len & 0xff
#define MAKE_PAT_TSID(p, ts_id)     PAT_P_IN_TP(p)[3] = (ts_id >> 8) & 0xff; PAT_P_IN_TP(p)[4] = ts_id & 0xff

#define GET_PAT_SECLEN(p)           ((PAT_P_IN_TP(p)[1] & 0x0f) << 8) | PAT_P_IN_TP(p)[2]

/* ****************************************************************************************************************** */

typedef struct _ixstb_fakepat_t {
    struct list_head    list;

    void        *fake_pat;
    int         fd;
} ixstb_fakepat_t;

/* ****************************************************************************************************************** */

static ixstb_fakepat_t  fakepat_list_;

/* ****************************************************************************************************************** */

static inline ixstb_fakepat_t* find_fakepat(void *fake_pat)
{
    ixstb_fakepat_t *pos;

    list_for_each_entry(pos, &fakepat_list_.list, list) {
        if (fake_pat == pos->fake_pat) {
            return pos;
        }
    }

    return NULL;
}

int ixstb_fakepat_init()
{
    INIT_LIST_HEAD(&fakepat_list_.list);
    return 0;
}

ix_fakepat_handle_t ixstb_fakepat_create(uint16_t ts_id, int fd)
{
    ixstb_fakepat_t *fake_pat;
    uint8_t *p;
    int     sec_len = PAT_BONE_LEN + 4/*crc*/ - 3/*header*/;

    p = (uint8_t*)malloc(188);
    memset(p, 0xff, 188);

    MAKE_TP_HEADER(p);
    MAKE_PAT_BONE(p);
    MAKE_PAT_SECLEN(p, sec_len);
    MAKE_PAT_TSID(p, ts_id);

    fake_pat = (ixstb_fakepat_t*)calloc(1, sizeof(ixstb_fakepat_t));
    fake_pat->fake_pat = (void*)p;
    fake_pat->fd = fd;

    list_add_tail(&fake_pat->list, &fakepat_list_.list);

    return (ix_fakepat_handle_t)p;
}

int ixstb_fakepat_delete(ix_fakepat_handle_t handle)
{
    void    *p = (void*)handle;

    if (p) {
        ixstb_fakepat_t *fake_pat = find_fakepat(p);

        if (fake_pat) {
            list_del(&fake_pat->list);
            free(fake_pat);
        }

        free(p);
    }

    return 0;
}

int ixstb_fakepat_add_pmt(ix_fakepat_handle_t handle, uint16_t program_number, uint16_t pmt_pid)
{
    uint8_t     *p = (uint8_t*)handle;
    int         ins_pos;
    int         sec_len = GET_PAT_SECLEN(p);
    uint32_t    crc;

    ins_pos = sec_len - 4/*crc*/ + 3/*header*/;

    PAT_P_IN_TP(p)[ins_pos + 0] = (program_number >> 8) & 0xff;
    PAT_P_IN_TP(p)[ins_pos + 1] = program_number & 0xff;
    PAT_P_IN_TP(p)[ins_pos + 2] = 0xe0 | ((pmt_pid >> 8) & 0x1f);
    PAT_P_IN_TP(p)[ins_pos + 3] = pmt_pid & 0xff;

    sec_len += 4;
    MAKE_PAT_SECLEN(p, sec_len);

    crc = crc32(CRC32_INIT, PAT_P_IN_TP(p), sec_len - 4/*crc*/ + 3/*header*/);

    PAT_P_IN_TP(p)[ins_pos + 4] = (crc >> 24) & 0xff;
    PAT_P_IN_TP(p)[ins_pos + 5] = (crc >> 16) & 0xff;
    PAT_P_IN_TP(p)[ins_pos + 6] = (crc >> 8) & 0xff;
    PAT_P_IN_TP(p)[ins_pos + 7] = crc & 0xff;

    return 0;
}

uint8_t* ixstb_fakepat_get_packet(int fd)
{
    ixstb_fakepat_t *pos;

    list_for_each_entry(pos, &fakepat_list_.list, list) {
        if (fd == pos->fd) {
            return (uint8_t*)pos->fake_pat;
        }
    }

    return NULL;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
