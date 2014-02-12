/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXCGI_MAKEPAGE_H_
#define IXCGI_MAKEPAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include "ixcgi_types.h"
#include <cJSON/cJSON.h>

/* ****************************************************************************************************************** */

typedef enum _ixcgi_error_t {
    _error_stbd_timeout,
    _error_db_busy,
    _error_illegal_data,
    _error_unknown
} ixcgi_error_t;

/* ****************************************************************************************************************** */

extern cJSON*   ixcgi_make_error_page(ixcgi_error_t err);
extern void ixcgi_make_raw_page(char *out);
extern int  ixcgi_make_text_page(cJSON *json);
extern int  ixcgi_make_html_page(cJSON *json);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXCGI_MAKEPAGE_H_ */

/* end of file ****************************************************************************************************** */
