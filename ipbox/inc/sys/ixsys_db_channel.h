/* ******************************************************************************************************************
 * Copyright (c) 2013, JBSignal Co.
 * All rights reserved.
 * ******************************************************************************************************************
 * wuyong.yi@jbsignal.com / 2013.2.19.
 * ******************************************************************************************************************
 * DVB/ATSC channel structure
 * ****************************************************************************************************************** */

#ifndef IXSYS_DB_CHANNEL_H_
#define IXSYS_DB_CHANNEL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <linux/dvb/frontend.h>

/* ****************************************************************************************************************** */

enum { CHANNEL_LIMIT = 10000 };
enum { CHANNEL_NAME_STRLEN = 36 };

typedef struct _ix_channel_t {
    uint32_t    ch_id;
    char        name[CHANNEL_NAME_STRLEN];

    fe_type_t   fe_type;

    uint16_t    vid_pid;
    uint16_t    aud_pid;
    uint16_t    pcr_pid;

    uint8_t     vid_type;
    uint8_t     aud_type;

    uint16_t    service_id;
    uint16_t    cas_id;

    uint32_t    favorite;
    uint32_t    rf_id;
} ix_channel_t;

/* ****************************************************************************************************************** */

extern int  ixsys_get_channel(char *db_path, int id, ix_channel_t *channel);
extern int  ixsys_add_channel(char *db_path, ix_channel_t *channel);
extern int  ixsys_update_channel(char *db_path, ix_channel_t *channel);
extern int  ixsys_delete_channel(char *db_path, int id);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSYS_DB_DVBS_H_ */

/* end of file ****************************************************************************************************** */
