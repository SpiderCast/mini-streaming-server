/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include <sqlite3/sqlite3.h>
#include "ixsys_db_dvbs.h"
#include "ixsys_print.h"
#include "ixsys_getstr.h"

/* ****************************************************************************************************************** */

int ixsys_get_dvbs(char *db_path, int id, ix_dvbs_t *dvbs)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    dbq[128];
    int     cols = 0;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "SELECT * FROM \"dvbs\" WHERE \"sat_id\" = '%d'", id);

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            dvbs->sat_id      = sqlite3_column_int(stmt, cols++);
            strcpy(dvbs->name, (char*)sqlite3_column_text(stmt, cols++));
            dvbs->lnb         = ixsys_get_lnb_str((char*)sqlite3_column_text(stmt, cols++));
            dvbs->lnbf_lo     = sqlite3_column_int(stmt, cols++);
            dvbs->lnbf_hi     = sqlite3_column_int(stmt, cols++);
            dvbs->lnbf_ref    = sqlite3_column_int(stmt, cols++);
            dvbs->lnbp        = ixsys_get_lnbp_str((char*)sqlite3_column_text(stmt, cols++));
            dvbs->_22khz      = ixsys_get_22khz_str((char*)sqlite3_column_text(stmt, cols++));
            dvbs->diseqc      = ixsys_get_diseqc_str((char*)sqlite3_column_text(stmt, cols++));
            dvbs->diseqc_port = sqlite3_column_int(stmt, cols++);
            dvbs->sat_pos     = sqlite3_column_int(stmt, cols++);
            dvbs->favorite    = sqlite3_column_int(stmt, cols++);
            dvbs->key_tp      = sqlite3_column_int(stmt, cols++);

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return 0;
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return -1;
}

int ixsys_add_dvbs(char *db_path, ix_dvbs_t *dvbs)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    *errmsg;
    char    dbq[256];
    int     sat_id = 0;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "SELECT * FROM \"dvbs\" WHERE \"name\" = '%s' AND \"sat_pos\" = '%d'", dvbs->name, dvbs->sat_pos);

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            dvbs->sat_id = sqlite3_column_int(stmt, 0);

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return ixsys_update_dvbs(db_path, dvbs);
        }

        sqlite3_finalize(stmt);
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "INSERT INTO \"dvbs\" (\"name\", \"lnb\", \"lnbf_lo\", \"lnbf_hi\", \"lnbf_ref\", "  \
                               "\"lnbp\", \"_22khz\", \"diseqc\", \"diseqc_port\", \"sat_pos\") "                   \
                               "VALUES ('%s', '%s', '%d', '%d', '%d', '%s', '%s', '%s', '%d', '%d')"
                             , dvbs->name, ixsys_getstr_lnb(dvbs->lnb), dvbs->lnbf_lo, dvbs->lnbf_hi, dvbs->lnbf_ref
                             , ixsys_getstr_lnbp(dvbs->lnbp), ixsys_getstr_22khz(dvbs->_22khz), ixsys_getstr_diseqc(dvbs->diseqc)
                             , dvbs->diseqc_port, dvbs->sat_pos);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    /* last record's "sat_id" return */

    strcpy(dbq, "SELECT * FROM \"dvbs\" ORDER BY \"sat_id\" DESC LIMIT '1'");

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE)
            sat_id = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return sat_id;
}

int ixsys_update_dvbs(char *db_path, ix_dvbs_t *dvbs)
{
    sqlite3 *db;
    char    *errmsg;
    char    dbq[256];

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "UPDATE \"dvbs\" SET \"name\" = '%s', \"lnb\" = '%s', \"lnbf_lo\" = '%d', \"lnbf_hi\" = '%d', "          \
                               "\"lnbf_ref\" = '%d', \"lnbp\" = '%s', \"_22khz\" = '%s', \"diseqc\" = '%s', \"diseqc_port\" = '%d', "   \
                               "\"sat_pos\" = '%d', \"favorite\" = '%d', \"key_tp\" = '%d' WHERE \"sat_id\" = '%d'"
                             , dvbs->name, ixsys_getstr_lnb(dvbs->lnb), dvbs->lnbf_lo, dvbs->lnbf_hi, dvbs->lnbf_ref
                             , ixsys_getstr_lnbp(dvbs->lnbp), ixsys_getstr_22khz(dvbs->_22khz), ixsys_getstr_diseqc(dvbs->diseqc)
                             , dvbs->diseqc_port, dvbs->sat_pos, dvbs->favorite, dvbs->key_tp, dvbs->sat_id);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_close(db);

    return dvbs->sat_id;
}

int ixsys_delete_dvbs(char *db_path, int id)
{
    sqlite3 *db;
    char    *errmsg;
    char    dbq[128];

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "DELETE FROM \"dvbs\" WHERE \"sat_id\" = '%d'", id);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_close(db);

    return id;
}

int ixsys_get_dvbs_tp(char *db_path, int id, ix_dvbs_tp_t *dvbs_tp)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    dbq[128];
    int     cols = 0;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "SELECT * FROM \"dvbs_tp\" WHERE \"rf_id\"='%d'", id);

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            dvbs_tp->rf_id      = sqlite3_column_int(stmt, cols++);
            dvbs_tp->freq       = sqlite3_column_int(stmt, cols++);
            dvbs_tp->sym        = sqlite3_column_int(stmt, cols++);
            dvbs_tp->polar      = ixsys_get_polar_str((char*)sqlite3_column_text(stmt, cols++));
            dvbs_tp->system     = ixsys_get_delivery_system_str((char*)sqlite3_column_text(stmt, cols++));
            dvbs_tp->modulation = ixsys_get_modulation_str((char*)sqlite3_column_text(stmt, cols++));
            dvbs_tp->org_netid  = sqlite3_column_int(stmt, cols++);
            dvbs_tp->netid      = sqlite3_column_int(stmt, cols++);
            dvbs_tp->tsid       = sqlite3_column_int(stmt, cols++);
            dvbs_tp->has_cach   = sqlite3_column_int(stmt, cols++);
            dvbs_tp->favorite   = sqlite3_column_int(stmt, cols++);
            dvbs_tp->sat_id     = sqlite3_column_int(stmt, cols++);

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return 0;
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return -1;
}

int ixsys_add_dvbs_tp(char *db_path, ix_dvbs_tp_t *dvbs_tp)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    *errmsg;
    char    dbq[256];
    int     rf_id = -1;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "SELECT * FROM \"dvbs_tp\" WHERE \"freq\" = '%d' AND \"sym\" = '%d' AND \"polar\" = '%s' AND \"sat_id\" = '%d'"
                             , dvbs_tp->freq, dvbs_tp->sym, ixsys_getstr_polar(dvbs_tp->polar), dvbs_tp->sat_id);

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            dvbs_tp->rf_id = sqlite3_column_int(stmt, 0);

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return ixsys_update_dvbs_tp(db_path, dvbs_tp);
        }

        sqlite3_finalize(stmt);
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "INSERT INTO \"dvbs_tp\" (\"freq\", \"sym\", \"polar\", \"system\", \"modulation\", \"org_netid\", \"netid\", \"tsid\", \"sat_id\") "    \
                               "VALUES ('%d', '%d', '%s', '%s', '%s', '%d', '%d', '%d', '%d')"
                             , dvbs_tp->freq, dvbs_tp->sym, ixsys_getstr_polar(dvbs_tp->polar)
                             , ixsys_getstr_delivery_system(dvbs_tp->system), ixsys_getstr_modulation(dvbs_tp->modulation)
                             , dvbs_tp->org_netid, dvbs_tp->netid, dvbs_tp->tsid, dvbs_tp->sat_id);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    /* last record's "sat_id" return */

    strcpy(dbq, "SELECT * FROM \"dvbs_tp\" ORDER BY \"rf_id\" DESC LIMIT '1'");

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE)
            rf_id = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return rf_id;
}

int ixsys_update_dvbs_tp(char *db_path, ix_dvbs_tp_t *dvbs_tp)
{
    sqlite3 *db;
    char    *errmsg;
    char    dbq[256];

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "UPDATE \"dvbs_tp\" SET \"freq\" = '%d', \"sym\" = '%d', \"polar\" = '%s', \"system\" = '%s', "  \
                               "\"modulation\" = '%s', \"org_netid\" = '%d', \"netid\" = '%d', \"tsid\" = '%d', \"favorite\" = '%d' WHERE \"rf_id\" = '%d'"
                             , dvbs_tp->freq, dvbs_tp->sym, ixsys_getstr_polar(dvbs_tp->polar)
                             , ixsys_getstr_delivery_system(dvbs_tp->system), ixsys_getstr_modulation(dvbs_tp->modulation)
                             , dvbs_tp->org_netid, dvbs_tp->netid, dvbs_tp->tsid, dvbs_tp->favorite, dvbs_tp->rf_id);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_close(db);

    return dvbs_tp->rf_id;
}

int ixsys_delete_dvbs_tp(char *db_path, int id)
{
    sqlite3 *db;
    char    *errmsg;
    char    dbq[128];

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "DELETE FROM \"dvbs_tp\" WHERE \"rf_id\" = '%d'", id);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_close(db);

    return id;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
