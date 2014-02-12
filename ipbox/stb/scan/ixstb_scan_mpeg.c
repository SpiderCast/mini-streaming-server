/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_scan_mpeg.h"
#include "ixsys_print.h"
#include <libucsi/mpeg/section.h>

/* ****************************************************************************************************************** */

ix_section_handle_t ixstb_scan_section_pat(int fe_id, uint8_t *buffer, size_t buf_size, ix_section_cb cb, void *priv)
{
    uint8_t f_bytes[1], f_masks[1];
    ix_section_handle_t handle;
    ix_section_param_t  param;

    f_bytes[0] = stag_mpeg_program_association;
    f_masks[0] = 0xff;

    param.pid      = TRANSPORT_PAT_PID;
    param.f_bytes  = f_bytes;
    param.f_masks  = f_masks;
    param.f_size   = sizeof(f_bytes);
    param.timeout  = PAT_TIMEOUT;
    param.buffer   = buffer;
    param.buf_size = buf_size;
    param.oneshot  = true;

    handle = ixstb_section_start(fe_id, &param, cb, priv);

    return handle;
}

ix_section_handle_t ixstb_scan_section_pmt(int fe_id, uint16_t pid, uint16_t program_number, uint8_t *buffer, size_t buf_size, ix_section_cb cb, void *priv)
{
    uint8_t f_bytes[3], f_masks[3];
    ix_section_handle_t handle;
    ix_section_param_t  param;

    f_bytes[0] = stag_mpeg_program_map;
    f_masks[0] = 0xff;
    f_bytes[1] = (program_number >> 8) & 0xff;
    f_masks[1] = 0xff;
    f_bytes[2] = program_number & 0xff;
    f_masks[2] = 0xff;

    param.pid      = pid;
    param.f_bytes  = f_bytes;
    param.f_masks  = f_masks;
    param.f_size   = sizeof(f_bytes);
    param.timeout  = PMT_TIMEOUT;
    param.buffer   = buffer;
    param.buf_size = buf_size;
    param.oneshot  = true;

    handle = ixstb_section_start(fe_id, &param, cb, priv);

    return handle;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
