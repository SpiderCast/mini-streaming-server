/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSTB_FE_H_
#define IXSTB_FE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <linux/dvb/frontend.h>

/* ****************************************************************************************************************** */

extern int  ixstb_fe_init();
extern int  ixstb_fe_update();
extern int  ixstb_fe_get_path(int id, char *path, size_t len);
extern int  ixstb_fe_get_name(int id, char *name, size_t len);
extern int  ixstb_fe_get_type(int id, fe_type_t *type);
extern int  ixstb_fe_get_desc(int id, int *fd);
extern int  ixstb_fe_get_devid(int id, int16_t *adapter, int16_t *frontend);
extern int  ixstb_fe_try_lock(int id, int rf_id);
extern int  ixstb_fe_unlock(int id);
extern int  ixstb_fe_get_param(int id, struct dvb_frontend_parameters *param);
extern fe_status_t  ixstb_fe_check_lock(int id, int timeout);
extern fe_status_t  ixstb_fe_get_signal_status(int id, int *ber, int *pwr, int *snr, int *unc);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSTB_FE_H_ */

/* end of file ****************************************************************************************************** */
