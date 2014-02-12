/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSTB_FAKEPAT_H_
#define IXSTB_FAKEPAT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"

/* ****************************************************************************************************************** */

typedef void*   ix_fakepat_handle_t;

/* ****************************************************************************************************************** */

extern int      ixstb_fakepat_init();
extern ix_fakepat_handle_t  ixstb_fakepat_create(uint16_t ts_id, int fd);
extern int      ixstb_fakepat_delete(ix_fakepat_handle_t handle);
extern int      ixstb_fakepat_add_pmt(ix_fakepat_handle_t handle, uint16_t program_number, uint16_t pmt_pid);
extern uint8_t* ixstb_fakepat_get_packet(int fd);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSTB_FAKEPAT_H_ */

/* end of file ****************************************************************************************************** */
