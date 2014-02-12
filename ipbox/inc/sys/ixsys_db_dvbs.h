/* ******************************************************************************************************************
 * Copyright (c) 2013, JBSignal Co.
 * All rights reserved.
 * ******************************************************************************************************************
 * wuyong.yi@jbsignal.com / 2013.2.19.
 * ******************************************************************************************************************
 * DVB-S/S2 data structure
 * ****************************************************************************************************************** */

#ifndef IXSYS_DB_DVBS_H_
#define IXSYS_DB_DVBS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <linux/dvb/frontend.h>

/* ****************************************************************************************************************** */

enum { DVBS_SAT_LIMIT = 200 };
enum { DVBS_TP_LIMIT = 10000 };

enum { DVBS_SATNAME_STRLEN = 40 };

typedef enum _ix_lnb_t {
    _lnb_universal,
    _lnb_single,
    _lnb_user,
    _lnb_end
} ix_lnb_t;

typedef enum _ix_lnbp_t {
    _lnbp_on,
    _lnbp_13v,
    _lnbp_18v,
    _lnbp_off,
    _lnbp_end
} ix_lnbp_t;

typedef enum _ix_22khz_t {
    _22khz_auto,
    _22khz_on,
    _22khz_off,
    _22khz_end
} ix_22khz_t;

typedef enum _ix_diseqc_t {
    _diseqc_none,
    _diseqc_10,
    _diseqc_11,
    _diseqc_motor,
    _diseqc_end
} ix_diseqc_t;

typedef struct _ix_dvbs_t {
    uint32_t    sat_id;

    char        name[DVBS_SATNAME_STRLEN];

    ix_lnb_t    lnb;

    int16_t     lnbf_lo;
    int16_t     lnbf_hi;
    int16_t     lnbf_ref;       // reference frequency : 11700 (universal LNB)

    ix_lnbp_t   lnbp;
    ix_22khz_t  _22khz;
    ix_diseqc_t diseqc;

    int16_t     diseqc_port;    // DiSEqC 1.0 port (1~4), DiSEqC 1.1 port (1~16), DiSEqC 1.2 motor position
    int16_t     sat_pos;

    uint32_t    favorite;

    int16_t     key_tp;
} ix_dvbs_t;

/* ****************************************************************************************************************** */

typedef enum _ix_polar_t {
    _polar_ver,                 // 13V
    _polar_hor,                 // 18V
    _polar_end
} ix_polar_t;

typedef struct _ix_dvbs_tp_t {
    uint32_t    rf_id;

    int16_t     freq;
    int16_t     sym;

    ix_polar_t  polar;

    fe_delivery_system_t    system;
    fe_modulation_t         modulation;

    uint16_t    org_netid;
    uint16_t    netid;
    uint16_t    tsid;

    bool        has_cach;
    uint32_t    favorite;

    uint32_t    sat_id;
} ix_dvbs_tp_t;

/* ****************************************************************************************************************** */

extern int  ixsys_get_dvbs(char *db_path, int id, ix_dvbs_t *dvbs);
extern int  ixsys_add_dvbs(char *db_path, ix_dvbs_t *dvbs);
extern int  ixsys_update_dvbs(char *db_path, ix_dvbs_t *dvbs);
extern int  ixsys_delete_dvbs(char *db_path, int id);
extern int  ixsys_get_dvbs_tp(char *db_path, int id, ix_dvbs_tp_t *dvbs_tp);
extern int  ixsys_add_dvbs_tp(char *db_path, ix_dvbs_tp_t *dvbs_tp);
extern int  ixsys_update_dvbs_tp(char *db_path, ix_dvbs_tp_t *dvbs_tp);
extern int  ixsys_delete_dvbs_tp(char *db_path, int id);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSYS_DB_DVBS_H_ */

/* end of file ****************************************************************************************************** */
