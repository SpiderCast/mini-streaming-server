/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXCGI_SQL_H_
#define IXCGI_SQL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include "ix_jsonop.h"
#include "ixcgi_types.h"

/* ****************************************************************************************************************** */

extern cJSON*   ixcgi_sql_dvbs_list(char *db_path);
extern cJSON*   ixcgi_sql_dvbs_tp_list(char *db_path, int sat_id);
extern cJSON*   ixcgi_sql_dvbs(char *db_path, int sat_id);
extern cJSON*   ixcgi_sql_dvbs_tp(char *db_path, int rf_id);
extern cJSON*   ixcgi_sql_channel_list(char *db_path, int fetype, int rf_id);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXCGI_SQL_H_ */

/* end of file ****************************************************************************************************** */
