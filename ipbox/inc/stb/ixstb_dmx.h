/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSTB_DMX_H_
#define IXSTB_DMX_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <linux/dvb/dmx.h>

/* ****************************************************************************************************************** */

#define DMX_PID_NULL            0x2000
#define DMX_SECTION_LIMIT       20

/* ****************************************************************************************************************** */

extern int  ixstb_dmx_set_section_filter(int fe_id, uint16_t pid, uint8_t *f_bytes, uint8_t *f_masks, size_t f_len, size_t buf_size, int timeout, bool oneshot);
extern int  ixstb_dmx_set_pid_filter(int fe_id, uint16_t pid, size_t buf_size, dmx_input_t input, dmx_output_t output, dmx_pes_type_t pes_type);
extern int  ixstb_dmx_clr_filter(int fd);
extern int  ixstb_dmx_start_filter(int fd);
extern int  ixstb_dmx_stop_filter(int fd);
extern int  ixstb_dmx_open_dvr(int fe_id, size_t buf_size);
extern int  ixstb_dmx_close_dvr(int fd);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSTB_DMX_H_ */

/* end of file ****************************************************************************************************** */
