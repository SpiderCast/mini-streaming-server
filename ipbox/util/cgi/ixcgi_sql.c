/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsys_db.h"
#include "ix_jsonop.h"
#include <sqlite3/sqlite3.h>
#include "ixcgi_sql.h"
#include "ixsys_getstr.h"
#include "ixcgi_makepage.h"

/* ****************************************************************************************************************** */

cJSON* ixcgi_sql_dvbs_list(char *db_path)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    *query = "SELECT * FROM \"dvbs\"";
    int     n, cols, rows = 0;
    cJSON   *root = cJSON_CreateObject();
    cJSON   *array, *item;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        return ixcgi_make_error_page(_error_db_busy);
    }

    if (!root)
        return ixcgi_make_error_page(_error_unknown);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_DVBS_LIST));
    cJSON_AddItemToObject(root, STBD_DVBS_LIST_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));
    cJSON_AddItemToObject(root, STBD_DVBS_LIST_COUNT, cJSON_CreateNumber(0));
    cJSON_AddItemToObject(root, STBD_DVBS_LIST_DVBS, array = cJSON_CreateArray());

    if (sqlite3_prepare(db, query, strlen(query), &stmt, NULL) == SQLITE_OK) {
        cols = sqlite3_column_count(stmt);

        while (1) {
            if (sqlite3_step(stmt) == SQLITE_DONE)
                break;

            item = cJSON_CreateObject();

            for (n = 0; n < cols; n++) {
                switch (sqlite3_column_type(stmt, n)) {
                default:
                case SQLITE_TEXT:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateString((char*)sqlite3_column_text(stmt, n)));
                    break;
                case SQLITE_INTEGER:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateNumber(sqlite3_column_int(stmt, n)));
                    break;
                }
            }

            cJSON_AddItemToArray(array, item);
            ++rows;
        }

        cJSON_ReplaceItemInObject(root, STBD_DVBS_LIST_COUNT, cJSON_CreateNumber(rows));

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return root;
}

cJSON* ixcgi_sql_dvbs_tp_list(char *db_path, int sat_id)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    query[80];
    int     n, cols, rows = 0;
    cJSON   *root = cJSON_CreateObject();
    cJSON   *array, *item;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        return ixcgi_make_error_page(_error_db_busy);
    }

    if (!root)
        return ixcgi_make_error_page(_error_unknown);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_DVBS_TP_LIST));
    cJSON_AddItemToObject(root, STBD_DVBS_TP_LIST_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));
    cJSON_AddItemToObject(root, STBD_DVBS_TP_LIST_COUNT, cJSON_CreateNumber(0));
    cJSON_AddItemToObject(root, STBD_DVBS_TP_LIST_DVBS_TP, array = cJSON_CreateArray());

    strcpy(query, "SELECT * FROM \"dvbs_tp\"");
    if (sat_id > 0)
        sprintf(query + strlen(query), " WHERE \"sat_id\" = '%d'", sat_id);

    if (sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL) == SQLITE_OK) {
        cols = sqlite3_column_count(stmt);

        while (1) {
            if (sqlite3_step(stmt) == SQLITE_DONE)
                break;

            item = cJSON_CreateObject();

            for (n = 0; n < cols; n++) {
                switch (sqlite3_column_type(stmt, n)) {
                default:
                case SQLITE_TEXT:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateString((char*)sqlite3_column_text(stmt, n)));
                    break;
                case SQLITE_INTEGER:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateNumber(sqlite3_column_int(stmt, n)));
                    break;
                }
            }

            cJSON_AddItemToArray(array, item);
            ++rows;
        }

        cJSON_ReplaceItemInObject(root, STBD_DVBS_TP_LIST_COUNT, cJSON_CreateNumber(rows));

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return root;
}

cJSON* ixcgi_sql_dvbs(char *db_path, int sat_id)
{
    sqlite3         *db;
    sqlite3_stmt    *stmt;
    char    query[80];
    int     n, cols;
    cJSON   *root = cJSON_CreateObject();
    cJSON   *item;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        return ixcgi_make_error_page(_error_db_busy);
    }

    if (!root)
        return ixcgi_make_error_page(_error_unknown);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_DVBS));
    cJSON_AddItemToObject(root, STBD_DVBS_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));
    cJSON_AddItemToObject(root, STBD_DVBS_DVBS, item = cJSON_CreateObject());

    memset(query, 0, sizeof(query));
    snprintf(query, 80, "SELECT * FROM \"dvbs\" WHERE \"sat_id\" = '%d'", sat_id);

    if (sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cols = sqlite3_column_count(stmt);
            for (n = 0; n < cols; n++) {
                switch (sqlite3_column_type(stmt, n)) {
                default:
                case SQLITE_TEXT:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateString((char*)sqlite3_column_text(stmt, n)));
                    break;
                case SQLITE_INTEGER:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateNumber(sqlite3_column_int(stmt, n)));
                    break;
                }
            }
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return root;
}

cJSON* ixcgi_sql_dvbs_tp(char *db_path, int rf_id)
{
    sqlite3 *db;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        return ixcgi_make_error_page(_error_db_busy);
    }

    sqlite3_stmt    *stmt;
    char    query[80];
    int     n, cols;
    cJSON   *root = cJSON_CreateObject();
    cJSON   *item;

    if (!root)
        return ixcgi_make_error_page(_error_unknown);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_DVBS_TP));
    cJSON_AddItemToObject(root, STBD_DVBS_TP_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));
    cJSON_AddItemToObject(root, STBD_DVBS_TP_DVBS_TP, item = cJSON_CreateObject());

    memset(query, 0, sizeof(query));
    snprintf(query, 80, "SELECT * FROM \"dvbs_tp\" WHERE \"rf_id\" = '%d'", rf_id);

    if (sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cols = sqlite3_column_count(stmt);
            for (n = 0; n < cols; n++) {
                switch (sqlite3_column_type(stmt, n)) {
                default:
                case SQLITE_TEXT:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateString((char*)sqlite3_column_text(stmt, n)));
                    break;
                case SQLITE_INTEGER:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateNumber(sqlite3_column_int(stmt, n)));
                    break;
                }
            }
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return root;
}

cJSON* ixcgi_sql_channel_list(char *db_path, int fetype, int rf_id)
{
    sqlite3 *db;

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        return ixcgi_make_error_page(_error_db_busy);
    }

    sqlite3_stmt    *stmt;
    char    query[128], str[8];
    int     n, cols, rows = 0;
    cJSON   *root = cJSON_CreateObject();
    cJSON   *array, *item;

    if (!root)
        return ixcgi_make_error_page(_error_unknown);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_CHANNEL_LIST));
    cJSON_AddItemToObject(root, STBD_CHANNEL_LIST_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));

    memset(query, 0, sizeof(query));
    strcpy(query, "SELECT * FROM \"channel\"");

    if (fetype >= 0) {
        strcpy(str, ixsys_getstr_fetype(fetype));
        cJSON_AddItemToObject(root, STBD_CHANNEL_LIST_FE_TYPE, cJSON_CreateString(str));
        sprintf(query + strlen(query), " WHERE \"fe_type\" = '%s'", str);

        if (rf_id > 0) {
            cJSON_AddItemToObject(root, STBD_CHANNEL_LIST_RF_ID, cJSON_CreateNumber(rf_id));
            sprintf(query + strlen(query), " AND \"rf_id\" = '%d'", rf_id);
        }
    }

    cJSON_AddItemToObject(root, STBD_CHANNEL_LIST_COUNT, cJSON_CreateNumber(0));
    cJSON_AddItemToObject(root, STBD_CHANNEL_LIST_CHANNEL, array = cJSON_CreateArray());

    if (sqlite3_prepare(db, query, strlen(query), &stmt, NULL) == SQLITE_OK) {
        cols = sqlite3_column_count(stmt);

        while (1) {
            if (sqlite3_step(stmt) == SQLITE_DONE)
                break;

            item = cJSON_CreateObject();

            for (n = 0; n < cols; n++) {
                switch (sqlite3_column_type(stmt, n)) {
                default:
                case SQLITE_TEXT:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateString((char*)sqlite3_column_text(stmt, n)));
                    break;
                case SQLITE_INTEGER:
                    cJSON_AddItemToObject(item, sqlite3_column_name(stmt, n), cJSON_CreateNumber(sqlite3_column_int(stmt, n)));
                    break;
                }
            }

            cJSON_AddItemToArray(array, item);
            ++rows;
        }

        cJSON_ReplaceItemInObject(root, STBD_SCAN_COUNT, cJSON_CreateNumber(rows));

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return root;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
