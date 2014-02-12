/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXCGI_REQSTB_H_
#define IXCGI_REQSTB_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include "ix_jsonop.h"
#include "ixsys_db_dvbs.h"
#include "ixcgi_types.h"

/* ****************************************************************************************************************** */

extern int  ixcgi_reqstb_system_info(char *in, size_t in_size);
extern int  ixcgi_reqstb_dvbs_list(char *in, size_t in_size);
extern int  ixcgi_reqstb_dvbs_tp_list(char *in, size_t in_size);
extern int  ixcgi_reqstb_dvbs(char *in, size_t in_size);
extern int  ixcgi_reqstb_dvbs_tp(char *in, size_t in_size);
extern int  ixcgi_reqstb_add_dvbs(ix_dvbs_t *dvbs, char *in, size_t in_size);
extern int  ixcgi_reqstb_update_dvbs(ix_dvbs_t *dvbs, char *in, size_t in_size);
extern int  ixcgi_reqstb_delete_dvbs(int sat_id, char *in, size_t in_size);
extern int  ixcgi_reqstb_add_dvbs_tp(ix_dvbs_tp_t *dvbs_tp, char *in, size_t in_size);
extern int  ixcgi_reqstb_update_dvbs_tp(ix_dvbs_tp_t *dvbs_tp, char *in, size_t in_size);
extern int  ixcgi_reqstb_delete_dvbs_tp(int rf_id, char *in, size_t in_size);
extern int  ixcgi_reqstb_try_lock(int fe_id, int rf_id, char *in, size_t in_size);
extern int  ixcgi_reqstb_lock_check(int fe_id, char *in, size_t in_size);
extern int  ixcgi_reqstb_move_to(int fe_id, int sat_id, char *type, char *param, char *longitude, char *latitude, char *in, size_t in_size);
extern int  ixcgi_reqstb_scan(int fe_id, int rf_id, char *in, size_t in_size);
extern int  ixcgi_reqstb_channel_list(int fe_id, int rf_id, char *in, size_t in_size);
extern int  ixcgi_reqstb_streaming(char *handle, char *type, int16_t port, int16_t ttl, int fe_id, int ch_id, char *path, char *in, size_t in_size);
extern int  ixcgi_reqstb_recording(char *handle, int fe_id, int ch_id, char *in, size_t in_size);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXCGI_REQSTB_H_ */

/* end of file ****************************************************************************************************** */
