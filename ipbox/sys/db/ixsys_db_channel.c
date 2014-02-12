/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include <sqlite3/sqlite3.h>
#include "ixsys_db_channel.h"
#include "ixsys_print.h"
#include "ixsys_getstr.h"

/* ****************************************************************************************************************** */

int ixsys_get_channel(char *db_path, int id, ix_channel_t *channel)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    dbq[256];
    int     cols = 0;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "SELECT * FROM \"channel\" WHERE \"ch_id\" = '%d'", id);

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            channel->ch_id      = sqlite3_column_int(stmt, cols++);
            strcpy(channel->name, (char*)sqlite3_column_text(stmt, cols++));
            channel->fe_type    = ixsys_get_fetype_str((char*)sqlite3_column_text(stmt, cols++));
            channel->vid_pid    = sqlite3_column_int(stmt, cols++);
            channel->aud_pid    = sqlite3_column_int(stmt, cols++);
            channel->pcr_pid    = sqlite3_column_int(stmt, cols++);
            channel->vid_type   = sqlite3_column_int(stmt, cols++);
            channel->aud_type   = sqlite3_column_int(stmt, cols++);
            channel->service_id = sqlite3_column_int(stmt, cols++);
            channel->cas_id     = sqlite3_column_int(stmt, cols++);
            channel->favorite   = sqlite3_column_int(stmt, cols++);
            channel->rf_id      = sqlite3_column_int(stmt, cols++);

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return 0;
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return -1;
}

int ixsys_add_channel(char *db_path, ix_channel_t *channel)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    *errmsg;
    char    dbq[512];
    int     ch_id = -1;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "SELECT * FROM \"channel\" WHERE \"service_id\" = '%d' AND \"rf_id\" = '%d'", channel->service_id, channel->rf_id);

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            channel->ch_id = sqlite3_column_int(stmt, 0);

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return ixsys_update_channel(db_path, channel);
        }

        sqlite3_finalize(stmt);
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "INSERT INTO \"channel\" (\"name\", \"fe_type\", \"vid_pid\", \"aud_pid\", \"pcr_pid\", "    \
                       "\"vid_type\", \"aud_type\", \"service_id\", \"cas_id\", \"rf_id\") "    \
                       "VALUES ('%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d')"
                       , channel->name, ixsys_getstr_fetype(channel->fe_type), channel->vid_pid, channel->aud_pid, channel->pcr_pid
                       , channel->vid_type, channel->aud_type, channel->service_id, channel->cas_id, channel->rf_id);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    /* last record's "ch_id" return */

    strcpy(dbq, "SELECT * FROM \"channel\" ORDER BY \"ch_id\" DESC LIMIT '1'");

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE)
            ch_id = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return ch_id;
}

int ixsys_update_channel(char *db_path, ix_channel_t *channel)
{
    sqlite3 *db;
    char    *errmsg;
    char    dbq[512];

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "UPDATE \"channel\" SET \"name\" = '%s', \"fe_type\" = '%s', "   \
                       "\"vid_pid\" = '%d', \"aud_pid\" = '%d', \"pcr_pid\" = '%d', \"vid_type\" = '%d', \"aud_type\" = '%d',"  \
                       "\"service_id\" = '%d', \"cas_id\" = '%d', \"favorite\" = '%d', \"rf_id\" = '%d' WHERE \"ch_id\" = '%d'"
                       , channel->name, ixsys_getstr_fetype(channel->fe_type), channel->vid_pid, channel->aud_pid, channel->pcr_pid
                       , channel->vid_type, channel->aud_type, channel->service_id, channel->cas_id, channel->favorite, channel->rf_id, channel->ch_id);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_close(db);

    return channel->ch_id;
}

int ixsys_delete_channel(char *db_path, int id)
{
    sqlite3 *db;
    char    *errmsg;
    char    dbq[256];

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, 128, "DELETE FROM \"channel\" WHERE \"ch_id\" = '%d'", id);

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
