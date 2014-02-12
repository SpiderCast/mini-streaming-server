/* ******************************************************************************************************************
 * Copyright (c) 2013, JBSignal Co.
 * All rights reserved.
 * ******************************************************************************************************************
 * wuyong.yi@jbsignal.com / 2013.2.28.
 * ****************************************************************************************************************** */

#ifndef IXSYS_GETSTR_H_
#define IXSYS_GETSTR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <linux/dvb/frontend.h>
#include "ixsys_db_dvbs.h"

/* ****************************************************************************************************************** */

extern char*    ixsys_getstr_fetype(fe_type_t type);
extern char*    ixsys_getstr_delivery_system(fe_delivery_system_t system);
extern char*    ixsys_getstr_modulation(fe_modulation_t modulation);
extern char*    ixsys_getstr_lnb(ix_lnb_t lnb);
extern char*    ixsys_getstr_lnbp(ix_lnbp_t lnbp);
extern char*    ixsys_getstr_22khz(ix_22khz_t _22khz);
extern char*    ixsys_getstr_diseqc(ix_diseqc_t diseqc);
extern char*    ixsys_getstr_polar(ix_polar_t polar);
extern char*    ixsys_getstr_streamingtype(ix_streaming_t type);
extern fe_type_t    ixsys_get_fetype_str(char *str);
extern fe_delivery_system_t ixsys_get_delivery_system_str(char *str);
extern fe_modulation_t      ixsys_get_modulation_str(char *str);
extern ix_lnb_t     ixsys_get_lnb_str(char *str);
extern ix_lnbp_t    ixsys_get_lnbp_str(char *str);
extern ix_22khz_t   ixsys_get_22khz_str(char *str);
extern ix_diseqc_t  ixsys_get_diseqc_str(char *str);
extern ix_polar_t   ixsys_get_polar_str(char *str);
extern ix_streaming_t   ixsys_get_streamingtype_str(char *str);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSYS_GETSTR_H_ */

/* end of file ****************************************************************************************************** */
