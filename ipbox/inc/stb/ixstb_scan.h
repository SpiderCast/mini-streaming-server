/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSTB_SCAN_H_
#define IXSTB_SCAN_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"

/* ****************************************************************************************************************** */

typedef enum _ix_scan_step_t {
    _scan_step_timeout,
    _scan_step_error,
    _scan_step_receive_pat,
    _scan_step_receive_sdt,
    _scan_step_receive_pmt,
    _scan_step_add_channel,
    _scan_step_done,
    _scan_step_end
} ix_scan_step_t;

typedef void    (*ix_scan_cb)(ix_scan_step_t step, void *param, size_t param_size, void *priv);

typedef struct _ix_scan_param_t {
    ix_scan_cb      cb;
    char            db_path[128];
    void            *priv;
} ix_scan_param_t;

typedef struct _ix_scan_handle_t {
    void        *local;
} *ix_scan_handle_t;

/* ****************************************************************************************************************** */

extern ix_scan_handle_t ixstb_scan_request(int fe_id, int rf_id, ix_scan_param_t *param);
extern int  ixstb_scan_retract(ix_scan_handle_t handle);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSTB_SCAN_H_ */

/* end of file ****************************************************************************************************** */
