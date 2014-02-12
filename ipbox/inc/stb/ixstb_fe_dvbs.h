/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSTB_FE_DVBS_H_
#define IXSTB_FE_DVBS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include "ixsys_db_dvbs.h"

/* ****************************************************************************************************************** */

typedef enum _ix_usals_cmd_t {
    _usals_step,
    _usals_angle,
    _usals_goto_n,
    _usals_store_n,
    _usals_auto,
    _usals_stop,
    _usals_end,
} ix_usals_cmd_t;

/* ****************************************************************************************************************** */

extern int  ixstb_fe_dvbs_locking(int fd, ix_dvbs_t *dvbs, ix_dvbs_tp_t *tp);
extern int  ixstb_fe_dvbs_unlocking(int fd);
extern int  ixstb_fe_dvbs_set_diseqc_motor(int fd, ix_usals_cmd_t cmd, int *param, int n_param);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSTB_FE_DVBS_H_ */

/* end of file ****************************************************************************************************** */
