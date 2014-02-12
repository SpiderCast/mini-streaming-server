/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_scan_dvb.h"
#include "ixsys_print.h"
#include <libucsi/dvb/section.h>

/* ****************************************************************************************************************** */

ix_section_handle_t ixstb_scan_section_sdt(int fe_id, uint16_t ts_id, uint8_t *buffer, size_t buf_size, ix_section_cb cb, void *priv)
{
    uint8_t f_bytes[3], f_masks[3];
    ix_section_handle_t handle;
    ix_section_param_t  param;

    f_bytes[0] = stag_dvb_service_description_actual;
    f_masks[0] = 0xff;
    f_bytes[1] = (ts_id >> 8) & 0xff;
    f_masks[1] = 0xff;
    f_bytes[2] = ts_id & 0xff;
    f_masks[2] = 0xff;

    param.pid      = TRANSPORT_SDT_PID;
    param.f_bytes  = f_bytes;
    param.f_masks  = f_masks;
    param.f_size   = sizeof(f_bytes);
    param.timeout  = SDT_TIMEOUT;
    param.buffer   = buffer;
    param.buf_size = buf_size;
    param.oneshot  = true;

    handle = ixstb_section_start(fe_id, &param, cb, priv);

    return handle;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
