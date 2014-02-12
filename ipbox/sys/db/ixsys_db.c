/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include <sqlite3/sqlite3.h>
#include "ixsys_db.h"
#include "ixsys_print.h"
#include "ixsys_db_table.h"
#include "ixsys_db_dvbs.h"
#include "ixsys_db_channel.h"

/* ****************************************************************************************************************** */

static int init_db(char *db_path)
{
    sqlite3 *db;
    char    *errmsg;
    char    dbq[128];

    print_debug("init DATABASE ...\n");

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_debug("sqlite DB create error");
        return -1;
    }

    /* create DB INFO table */

    if (sqlite3_exec(db, DBQ_DBINFO_TAB_CREAT, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    snprintf(dbq, sizeof(dbq), "INSERT INTO \"dbinfo\" (\"version\") VALUES ('%d')", DB_VERSION);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);

        sqlite3_close(db);
        unlink(DB_PATH_DATA);

        return -1;
    }

    sqlite3_close(db);

    return 0;
}

static int create_db(char *db_path)
{
    sqlite3 *db;
    char    *errmsg;

    print_debug("create DATABASE ...\n");

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_debug("sqlite DB create error");
        return -1;
    }

    /* create DVB-S/S2 table */

    if (sqlite3_exec(db, DBQ_DVBS_TAB_CREAT, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_error("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    /* create DVB-S/S2 TP table */

    if (sqlite3_exec(db, DBQ_DVBS_TP_TAB_CREAT, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_debug("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    /* create CHANNEL table */

    if (sqlite3_exec(db, DBQ_CHANNEL_TAB_CREAT, NULL, NULL, &errmsg) != SQLITE_OK) {
        print_debug("sqlite error: %s", errmsg);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_close(db);

    return init_db(DB_PATH_DATA);
}

static int recovery_db()
{
    if (access(DB_PATH_DEFAULT_DATA, F_OK) == 0) {
        system("cp -fp 2>/dev/null "DB_PATH_DEFAULT_DATA" "DB_PATH_DATA);
        return init_db(DB_PATH_DATA);
    }

    return -1;
}

int ixsys_db_init()
{
    int     fd;

    if ((fd = open(DB_PATH_DATA, O_RDONLY)) > 0) {
        close(fd);
    } else {
        if (recovery_db() != 0) {
            create_db(DB_PATH_DATA);
        }
    }

    return 0;
}

int ixsys_db_get_info(char *db_path, ix_dbinfo_t *db_info)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char            dbq[128];
    int             cols = 0;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        print_error("sqlite DB open error");
        return -1;
    }

    strcpy(dbq, "SELECT * FROM \"dbinfo\"");

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            db_info->version = sqlite3_column_int(stmt, cols++);

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            return 0;
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return -1;
}

int ixsys_set_user_position(int longitude, int latitude)
{
    print_debug("USER longitude : %d / latitude : %d\n", longitude, latitude);
    return 0;
}

int ixsys_get_user_position(int *longitude, int *latitude)
{
    *longitude = 1036;
    *latitude = 365;

    return 0;
}

int ixsys_get_epg_url(char *path, size_t length)
{
    strcpy(path, "http://172.16.10.4/webapi/epg");
    return 0;
}

int ixsys_get_epg_filepath(char *path, size_t length)
{
    strcpy(path, "/var/data/epg");
    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
