/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSTB_SCAN_MPEG_H_
#define IXSTB_SCAN_MPEG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include "ixstb_section.h"

/* ****************************************************************************************************************** */

#define PAT_TIMEOUT     3000
#define PMT_TIMEOUT     10000

/* ****************************************************************************************************************** */

extern ix_section_handle_t  ixstb_scan_section_pat(int fe_id, uint8_t *buffer, size_t buf_size, ix_section_cb cb, void *priv);
extern ix_section_handle_t  ixstb_scan_section_pmt(int fe_id, uint16_t pid, uint16_t program_number, uint8_t *buffer, size_t buf_size, ix_section_cb cb, void *priv);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSTB_SCAN_MPEG_H_ */

/* end of file ****************************************************************************************************** */
