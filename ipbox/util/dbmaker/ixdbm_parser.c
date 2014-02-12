/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <mxml/mxml.h>
#include <sqlite3/sqlite3.h>
#include "ixsys_db_dvbs.h"
#include "ixsys_getstr.h"
#include "ixsys_db_table.h"

/* ****************************************************************************************************************** */

static int create_table(sqlite3 *db)
{
    char    *errmsg;

    /* create DVB-S/S2 table */

    if (sqlite3_exec(db, DBQ_DVBS_TAB_CREAT, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "sqlite error: %s\n", errmsg);
        return -1;
    }

    /* create DVB-S/S2 TP table */

    if (sqlite3_exec(db, DBQ_DVBS_TP_TAB_CREAT, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "sqlite error: %s\n", errmsg);
        return -1;
    }

    /* create CHANNEL table */

    if (sqlite3_exec(db, DBQ_CHANNEL_TAB_CREAT, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "sqlite error: %s\n", errmsg);
        return -1;
    }

    return 0;
}

static int add_sat(sqlite3 *db, mxml_node_t *node)
{
    char    dbq[256];
    char    *errmsg;
    int     sat_pos = strtol(mxmlElementGetAttr(node, "position"), NULL, 10);
    int     sat_id = 0;
    sqlite3_stmt    *stmt;

    if (sat_pos < 0)
        sat_pos += 3600;

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "INSERT INTO \"dvbs\" (\"name\", \"sat_pos\") VALUES ('%s', '%d')"
                             , mxmlElementGetAttr(node, "name"), sat_pos);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "sqlite error: %s\n", errmsg);
        return -1;
    }

    /* last record's "sat_id" return */

    strcpy(dbq, "SELECT * FROM \"dvbs\" ORDER BY \"sat_id\" DESC LIMIT '1'");

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE)
            sat_id = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }

    // printf(">> Add SAT : [%d] %s [%d]\n", sat_id, mxmlElementGetAttr(node, "name"), sat_pos);

    return sat_id;
}

static int add_tp(sqlite3 *db, int sat_id, mxml_node_t *node)
{
    char    dbq[256];
    char    *errmsg;
    int     freq = strtol(mxmlElementGetAttr(node, "frequency"), NULL, 10) / 1000;
    int     sym = strtol(mxmlElementGetAttr(node, "symbol_rate"), NULL, 10) / 1000;
    int     n;
    sqlite3_stmt    *stmt;
    ix_polar_t      polar;
    fe_delivery_system_t    system;
    fe_modulation_t         modulation;

    switch ((n = strtol(mxmlElementGetAttr(node, "polarization"), NULL, 10))) {
    case 0:
        polar = _polar_hor;
        break;
    case 1:
        polar = _polar_ver;
        break;
    }

    switch ((n = strtol(mxmlElementGetAttr(node, "system"), NULL, 10))) {
    case 0:
        system = SYS_DVBS;
        break;
    case 1:
        system = SYS_DVBS2;
        break;
    }

    switch ((n = strtol(mxmlElementGetAttr(node, "modulation"), NULL, 10))) {
    case 0:
    case 1:
        modulation = QPSK;
        break;
    case 2:
        modulation = PSK_8;
        break;
    }

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "INSERT INTO \"dvbs_tp\" (\"freq\", \"sym\", \"polar\", \"system\", \"modulation\", \"sat_id\") "    \
                               "VALUES ('%d', '%d', '%s', '%s', '%s', '%d')"
                             , freq, sym, ixsys_getstr_polar(polar), ixsys_getstr_delivery_system(system)
                             , ixsys_getstr_modulation(modulation), sat_id);

    if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "sqlite error: %s\n", errmsg);
        return -1;
    }

    /* last record's "rf_id" return */

    strcpy(dbq, "SELECT * FROM \"dvbs_tp\" ORDER BY \"rf_id\" DESC LIMIT '1'");

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE)
            n = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }

    // printf(">>>> Add TP : [%d-%d] %d/%d/%c/%s/%s\n", sat_id, n, freq, sym, polar == _polar_ver ? 'V':'H', system == SYS_DVBS ? "DVB-S":"DVB-S2", modulation == QPSK ? "QPSK":"8PSK");

    return n;
}

static int set_keytp(sqlite3 *db, int sat_id)
{
    char    dbq[256];
    char    *errmsg;
    int     rf_id = 0;
    sqlite3_stmt    *stmt;

    memset(dbq, 0, sizeof(dbq));
    snprintf(dbq, sizeof(dbq), "SELECT * FROM \"dvbs_tp\" WHERE \"sat_id\" = '%d' ORDER BY \"rf_id\" ASC LIMIT '1'", sat_id);

    if (sqlite3_prepare_v2(db, dbq, strlen(dbq), &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) != SQLITE_DONE)
            rf_id = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }

    if (rf_id > 0) {
        memset(dbq, 0, sizeof(dbq));
        snprintf(dbq, sizeof(dbq), "UPDATE \"dvbs\" SET \"key_tp\" = '%d' WHERE \"sat_id\" = '%d'", rf_id, sat_id);

        if (sqlite3_exec(db, dbq, NULL, NULL, &errmsg) != SQLITE_OK) {
            fprintf(stderr, "sqlite error: %s", errmsg);
            return -1;
        }
    }

    return rf_id;
}

int ixdbm_parser(char *xml_fname, char *dst_fname)
{
#define TMP_XML_FNAME   "tmp.xml"

    FILE            *fp;
    mxml_node_t     *tree;
    mxml_node_t     *node_sat, *node_tp;
    int             sat_id;
    sqlite3         *mdb, *fdb;
    sqlite3_backup  *backup;
    char            cmd[128];

    snprintf(cmd, sizeof(cmd), "iconv -f ISO8859-1 -t UTF8 -o "TMP_XML_FNAME" %s", xml_fname);
    system(cmd);

    fp = fopen(TMP_XML_FNAME, "r");

    if ((tree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK)) == NULL) {
        fprintf(stderr, "illegal file structure\n");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    unlink(TMP_XML_FNAME);

    if (sqlite3_open_v2(":memory:", &mdb, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        fprintf(stderr, "sqlite MemoryDB open error\n");
        return -1;
    }

    if (create_table(mdb) != 0) {
        fprintf(stderr, "DB table creation error\n");
        sqlite3_close(mdb);
        return -1;
    }

    for (node_sat = mxmlFindElement(tree, tree, "sat", "name", NULL, MXML_DESCEND);
         node_sat != NULL;
         node_sat = mxmlFindElement(node_sat, tree, "sat", "name", NULL, MXML_DESCEND)) {
        if ((sat_id = add_sat(mdb, node_sat)) > 0) {
            for (node_tp = mxmlFindElement(node_sat, node_sat, "transponder", NULL, NULL, MXML_DESCEND);
                 node_tp != NULL;
                 node_tp = mxmlFindElement(node_tp, node_sat, "transponder", NULL, NULL, MXML_DESCEND)) {
                add_tp(mdb, sat_id, node_tp);
            }

            set_keytp(mdb, sat_id);
        }
    }

    if (sqlite3_open(dst_fname, &fdb) == SQLITE_OK) {
        if ((backup = sqlite3_backup_init(fdb, "main", mdb, "main")) != NULL) {
            sqlite3_backup_step(backup, -1);
            sqlite3_backup_finish(backup);
        }

        sqlite3_close(fdb);
    } else {
        fprintf(stderr, "sqlite FileDB open error\n");
    }

    sqlite3_close(mdb);
    mxmlDelete(tree);

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
