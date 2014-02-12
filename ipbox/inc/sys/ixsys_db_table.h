/* ******************************************************************************************************************
 * Copyright (c) 2013, JBSignal Co.
 * All rights reserved.
 * ******************************************************************************************************************
 * wuyong.yi@jbsignal.com / 2013.4.23.
 * ****************************************************************************************************************** */

#ifndef IXSYS_DB_TABLE_H_
#define IXSYS_DB_TABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#define DBQ_DBINFO_TAB_CREAT     "CREATE TABLE IF NOT EXISTS \"dbinfo\" (   \
                                \"version\" INTEGER DEFAULT 2014012101  \
                                );"

#define DBQ_DVBS_TAB_CREAT      "CREATE TABLE IF NOT EXISTS \"dvbs\" (  \
                                \"sat_id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE ,  \
                                \"name\" CHAR NOT NULL ,                \
                                \"lnb\" CHAR DEFAULT UNIVERSAL ,        \
                                \"lnbf_lo\" INTEGER DEFAULT 9750 ,      \
                                \"lnbf_hi\" INTEGER DEFAULT 10600 ,     \
                                \"lnbf_ref\" INTEGER DEFAULT 11700 ,    \
                                \"lnbp\" CHAR DEFAULT AUTO ,            \
                                \"_22khz\" CHAR DEFAULT AUTO ,          \
                                \"diseqc\" CHAR DEFAULT NONE ,          \
                                \"diseqc_port\" INTEGER DEFAULT 0 ,     \
                                \"sat_pos\" INTEGER DEFAULT 0 ,         \
                                \"favorite\" INTEGER DEFAULT 0,         \
                                \"key_tp\" INTEGER DEFAULT 0            \
                                );"

#define DBQ_DVBS_TP_TAB_CREAT   "CREATE TABLE IF NOT EXISTS \"dvbs_tp\" (   \
                                \"rf_id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE ,   \
                                \"freq\" INTEGER NOT NULL ,             \
                                \"sym\" INTEGER NOT NULL ,              \
                                \"polar\" CHAR DEFAULT HOR ,            \
                                \"system\" CHAR DEFAULT DVBS ,          \
                                \"modulation\" CHAR DEFAULT QPSK ,      \
                                \"org_netid\" INTEGER DEFAULT 0 ,       \
                                \"netid\" INTEGER DEFAULT 0 ,           \
                                \"tsid\" INTEGER DEFAULT 0 ,            \
                                \"has_cach\" INTEGER DEFAULT 0 ,        \
                                \"favorite\" INTEGER DEFAULT 0,         \
                                \"sat_id\" INTEGER NOT NULL             \
                                );"

#define DBQ_CHANNEL_TAB_CREAT   "CREATE TABLE IF NOT EXISTS \"channel\" (   \
                                \"ch_id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE ,   \
                                \"name\" CHAR NOT NULL ,            \
                                \"fe_type\" CHAR NOT NULL ,         \
                                \"vid_pid\" INTEGER DEFAULT 0,      \
                                \"aud_pid\" INTEGER DEFAULT 0,      \
                                \"pcr_pid\" INTEGER DEFAULT 0,      \
                                \"vid_type\" INTEGER DEFAULT 0,     \
                                \"aud_type\" INTEGER DEFAULT 0,     \
                                \"service_id\" INTEGER DEFAULT 0,   \
                                \"cas_id\" INTEGER DEFAULT 0,       \
                                \"favorite\" INTEGER DEFAULT 0,     \
                                \"rf_id\" INTEGER NOT NULL          \
                                );"

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSYS_DB_TABLE_H_ */

/* end of file ****************************************************************************************************** */
