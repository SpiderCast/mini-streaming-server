/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsvc_cmd_parser.h"
#include "ixsys_print.h"
#include "ix_jsonop.h"
#include "ixsys_sysinfo.h"
#include "ixsys_getstr.h"
#include "ixsys_db.h"
#include "ixsys_db_dvbs.h"
#include "ixsys_db_channel.h"
#include "ixstb_fe.h"
#include "ixstb_fe_dvbs.h"
#include "ixstb_scan.h"
#include "ixstb_player.h"

/* ****************************************************************************************************************** */

static cJSON* op_proc_system_info(cJSON *json)
{
    print_here();

    cJSON       *root = cJSON_CreateObject();
    cJSON       *array, *item;
    char        str[128];
    size_t      len = sizeof(str);
    int         n;
    fe_type_t   type;
    ix_dbinfo_t db_info;

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_SYSTEM_INFO));
    ixsys_get_ipaddr(NULL, str, len);
    cJSON_AddItemToObject(root, STBD_SYSTEM_INFO_MY_IP, cJSON_CreateString(str));
    ixsys_get_model(str, len);
    cJSON_AddItemToObject(root, STBD_SYSTEM_INFO_MODEL_NAME, cJSON_CreateString(str));
    ixsys_get_hwver(str, len);
    cJSON_AddItemToObject(root, STBD_SYSTEM_INFO_HW_VER, cJSON_CreateString(str));
    ixsys_get_swver(str, len);
    cJSON_AddItemToObject(root, STBD_SYSTEM_INFO_SW_VER, cJSON_CreateString(str));
    cJSON_AddItemToObject(root, STBD_SYSTEM_INFO_FE_COUNT, cJSON_CreateNumber(0));
    cJSON_AddItemToObject(root, STBD_SYSTEM_INFO_FE_LIST, array = cJSON_CreateArray());

    ixstb_fe_update();

    for (n = 0; n < 9; n++) {
        if (ixstb_fe_get_path(n, str, sizeof(str)) == -1)
            break;

        cJSON_AddItemToArray(array, item = cJSON_CreateObject());
        cJSON_AddItemToObject(item, STB_SYSTEM_INFO_FE_ID, cJSON_CreateNumber(n));
        cJSON_AddItemToObject(item, STBD_SYSTEM_INFO_FE_PATH, cJSON_CreateString(str));
        ixstb_fe_get_type(n, &type);
        cJSON_AddItemToObject(item, STBD_SYSTEM_INFO_FE_TYPE, cJSON_CreateString(ixsys_getstr_fetype(type)));
        ixstb_fe_get_name(n, str, sizeof(str));
        cJSON_AddItemToObject(item, STBD_SYSTEM_INFO_FE_MODEL_NAME, cJSON_CreateString(str));
    }

    cJSON_ReplaceItemInObject(root, STBD_SYSTEM_INFO_FE_COUNT, cJSON_CreateNumber(n));

    ixsys_db_get_info(DB_PATH_DATA, &db_info);

    cJSON_AddItemToObject(root, STBD_SYSTEM_INFO_DB, item = cJSON_CreateObject());
    cJSON_AddItemToObject(item, STBD_SYSTEM_INFO_DB_VERSION, cJSON_CreateNumber(db_info.version));

    system("cp -fp 2>/dev/null "DB_PATH_DATA" "DB_PATH_COPY);

    return root;
}

static cJSON* db_is_ready(char *path)
{
    cJSON   *root = cJSON_CreateObject();
    char    ipstr[16];

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_DB_READY));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_DB_READY_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_DB_READY_ANSWER, cJSON_CreateTrue());
    cJSON_AddItemToObject(root, STBD_DB_READY_PATH, cJSON_CreateString(path));

    return root;
}

static cJSON* op_proc_dvbs_list(cJSON *json)
{
    print_here();
    return db_is_ready(DB_PATH_DATA);
}

static cJSON* op_proc_dvbs_tp_list(cJSON *json)
{
    print_here();
    return db_is_ready(DB_PATH_DATA);
}

static cJSON* op_proc_dvbs(cJSON *json)
{
    print_here();
    return db_is_ready(DB_PATH_DATA);
}

static cJSON* op_proc_dvbs_tp(cJSON *json)
{
    print_here();
    return db_is_ready(DB_PATH_DATA);
}

static cJSON* op_proc_add_dvbs(cJSON *json)
{
    print_here();

    cJSON       *root = cJSON_CreateObject();
    cJSON       *item = cJSON_GetObjectItem(json, HTTPD_ADD_DVBS_DVBS);
    char        ipstr[16];
    ix_dvbs_t   dvbs;
    int         sat_id;

    memset(&dvbs, 0, sizeof(dvbs));
    strcpy(dvbs.name, cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_NAME)->valuestring);
    dvbs.lnb         = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_LNB)->valueint;
    dvbs.lnbf_lo     = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_LNBF_LO)->valueint;
    dvbs.lnbf_hi     = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_LNBF_HI)->valueint;
    dvbs.lnbf_ref    = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_LNBF_REF)->valueint;
    dvbs.lnbp        = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_LNBP)->valueint;
    dvbs.diseqc      = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_DISEQC)->valueint;
    dvbs.diseqc_port = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_DISEQC_PORT)->valueint;
    dvbs.sat_pos     = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_DVBS_SAT_POS)->valueint;

    sat_id = ixsys_add_dvbs(DB_PATH_DATA, &dvbs);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_ADD_DVBS));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_ADD_DVBS_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_ADD_DVBS_ANSWER, (sat_id > 0) ? cJSON_CreateTrue() : cJSON_CreateFalse());
    cJSON_AddItemToObject(root, STBD_ADD_DVBS_SAT_ID, cJSON_CreateNumber(sat_id));

    return root;
}

static cJSON* op_proc_update_dvbs(cJSON *json)
{
    print_here();

    cJSON       *root = cJSON_CreateObject();
    cJSON       *item = cJSON_GetObjectItem(json, HTTPD_UPDATE_DVBS_DVBS);
    char        ipstr[16];
    ix_dvbs_t   dvbs;
    int         sat_id;

    memset(&dvbs, 0, sizeof(dvbs));
    dvbs.sat_id      = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_SAT_ID)->valueint;
    strcpy(dvbs.name, cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_NAME)->valuestring);
    dvbs.lnb         = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_LNB)->valueint;
    dvbs.lnbf_lo     = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_LNBF_LO)->valueint;
    dvbs.lnbf_hi     = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_LNBF_HI)->valueint;
    dvbs.lnbf_ref    = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_LNBF_REF)->valueint;
    dvbs.lnbp        = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_LNBP)->valueint;
    dvbs.diseqc      = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_DISEQC)->valueint;
    dvbs.diseqc_port = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_DISEQC_PORT)->valueint;
    dvbs.sat_pos     = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_SAT_POS)->valueint;
    dvbs.key_tp      = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_DVBS_KEY_TP)->valueint;

    sat_id = ixsys_update_dvbs(DB_PATH_DATA, &dvbs);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_UPDATE_DVBS));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_UPDATE_DVBS_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_UPDATE_DVBS_ANSWER, (sat_id > 0) ? cJSON_CreateTrue() : cJSON_CreateFalse());
    cJSON_AddItemToObject(root, STBD_UPDATE_DVBS_SAT_ID, cJSON_CreateNumber(sat_id));

    return root;
}

static cJSON* op_proc_delete_dvbs(cJSON *json)
{
    print_here();

    cJSON   *root = cJSON_CreateObject();
    char    ipstr[16];
    int     sat_id;

    sat_id = ixsys_delete_dvbs(DB_PATH_DATA, cJSON_GetObjectItem(json, HTTPD_DELETE_DVBS_SAT_ID)->valueint);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_DELETE_DVBS));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_DELETE_DVBS_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_DELETE_DVBS_ANSWER, (sat_id > 0) ? cJSON_CreateTrue() : cJSON_CreateFalse());
    cJSON_AddItemToObject(root, STBD_DELETE_DVBS_SAT_ID, cJSON_CreateNumber(sat_id));

    return root;
}

static cJSON* op_proc_add_dvbs_tp(cJSON *json)
{
    print_here();

    cJSON       *root = cJSON_CreateObject();
    cJSON       *item = cJSON_GetObjectItem(json, HTTPD_ADD_DVBS_TP_DVBS_TP);
    char        ipstr[16];
    ix_dvbs_tp_t    dvbs_tp;
    int         rf_id;

    memset(&dvbs_tp, 0, sizeof(dvbs_tp));
    dvbs_tp.freq       = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_FREQ)->valueint;
    dvbs_tp.sym        = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_SYM)->valueint;
    dvbs_tp.polar      = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_POLAR)->valueint;
    dvbs_tp.system     = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_DELIVERY)->valueint;
    dvbs_tp.modulation = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_MODULATION)->valueint;
    dvbs_tp.org_netid  = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_ORG_NETID)->valueint;
    dvbs_tp.netid      = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_NETID)->valueint;
    dvbs_tp.tsid       = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_TSID)->valueint;
    dvbs_tp.sat_id     = cJSON_GetObjectItem(item, HTTPD_ADD_DVBS_TP_DVBS_TP_SAT_ID)->valueint;

    rf_id = ixsys_add_dvbs_tp(DB_PATH_DATA, &dvbs_tp);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_ADD_DVBS_TP));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_ADD_DVBS_TP_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_ADD_DVBS_TP_ANSWER, (rf_id > 0) ? cJSON_CreateTrue() : cJSON_CreateFalse());
    cJSON_AddItemToObject(root, STBD_ADD_DVBS_TP_RF_ID, cJSON_CreateNumber(rf_id));

    return root;
}

static cJSON* op_proc_update_dvbs_tp(cJSON *json)
{
    print_here();

    cJSON       *root = cJSON_CreateObject();
    cJSON       *item = cJSON_GetObjectItem(json, HTTPD_UPDATE_DVBS_TP_DVBS_TP);
    char        ipstr[16];
    ix_dvbs_tp_t    dvbs_tp;
    int         rf_id;

    memset(&dvbs_tp, 0, sizeof(dvbs_tp));
    dvbs_tp.rf_id      = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_RF_ID)->valueint;
    dvbs_tp.freq       = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_FREQ)->valueint;
    dvbs_tp.sym        = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_SYM)->valueint;
    dvbs_tp.polar      = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_POLAR)->valueint;
    dvbs_tp.system     = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_DELIVERY)->valueint;
    dvbs_tp.modulation = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_MODULATION)->valueint;
    dvbs_tp.org_netid  = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_ORG_NETID)->valueint;
    dvbs_tp.netid      = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_NETID)->valueint;
    dvbs_tp.tsid       = cJSON_GetObjectItem(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_TSID)->valueint;

    rf_id = ixsys_update_dvbs_tp(DB_PATH_DATA, &dvbs_tp);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_UPDATE_DVBS_TP));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_UPDATE_DVBS_TP_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_UPDATE_DVBS_TP_ANSWER, (rf_id > 0) ? cJSON_CreateTrue() : cJSON_CreateFalse());
    cJSON_AddItemToObject(root, STBD_UPDATE_DVBS_TP_RF_ID, cJSON_CreateNumber(rf_id));

    return root;
}

static cJSON* op_proc_delete_dvbs_tp(cJSON *json)
{
    print_here();

    cJSON   *root = cJSON_CreateObject();
    char    ipstr[16];
    int     rf_id;

    rf_id = ixsys_delete_dvbs_tp(DB_PATH_DATA, cJSON_GetObjectItem(json, HTTPD_DELETE_DVBS_TP_RF_ID)->valueint);

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_DELETE_DVBS_TP));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_DELETE_DVBS_TP_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_DELETE_DVBS_TP_ANSWER, (rf_id > 0) ? cJSON_CreateTrue() : cJSON_CreateFalse());
    cJSON_AddItemToObject(root, STBD_DELETE_DVBS_TP_RF_ID, cJSON_CreateNumber(rf_id));

    return root;
}

static cJSON* op_proc_try_lock(cJSON *json)
{
    print_here();

    cJSON   *root = cJSON_CreateObject();
    char    ipstr[16];
    int     fe_id = cJSON_GetObjectItem(json, HTTPD_TRY_LOCK_FE_ID)->valueint;
    int     rf_id = cJSON_GetObjectItem(json, HTTPD_TRY_LOCK_RF_ID)->valueint;
    bool    lock = false;
    int     ber, pwr, snr, unc;
    fe_status_t status;

    if (ixstb_fe_try_lock(fe_id, rf_id) == 0) {
        sleep(2);

        status = ixstb_fe_get_signal_status(fe_id, &ber, &pwr, &snr, &unc);
        if (status & FE_HAS_LOCK)
            lock = true;
    }

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_TRY_LOCK));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_TRY_LOCK_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_TRY_LOCK_ANSWER, cJSON_CreateFalse());

    if (lock == true) {
        cJSON_ReplaceItemInObject(root, STBD_TRY_LOCK_ANSWER, cJSON_CreateTrue());
        cJSON_AddItemToObject(root, STBD_TRY_LOCK_STRENGTH, cJSON_CreateNumber((pwr * 100) / 0xffff));
        cJSON_AddItemToObject(root, STBD_TRY_LOCK_SNR, cJSON_CreateNumber((snr * 100) / 0xffff));
        cJSON_AddItemToObject(root, STBD_TRY_LOCK_BER, cJSON_CreateNumber(ber));
        cJSON_AddItemToObject(root, STBD_TRY_LOCK_UNC, cJSON_CreateNumber(unc));
    }

    return root;
}

static cJSON* op_proc_lock_check(cJSON *json)
{
    print_here();

    cJSON       *root = cJSON_CreateObject();
    char        ipstr[16];
    int         fe_id = cJSON_GetObjectItem(json, HTTPD_LOCK_CHECK_FE_ID)->valueint;
    bool        lock = false;
    int         ber, pwr, snr, unc;
    fe_type_t   type;
    fe_status_t status;

    status = ixstb_fe_get_signal_status(fe_id, &ber, &pwr, &snr, &unc);
    if (status & FE_HAS_LOCK)
        lock = true;

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_LOCK_CHECK));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_LOCK_CHECK_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_LOCK_CHECK_ANSWER, cJSON_CreateFalse());
    cJSON_AddItemToObject(root, STBD_LOCK_CHECK_FE_ID, cJSON_CreateNumber(fe_id));

    if (lock == true) {
        cJSON_ReplaceItemInObject(root, STBD_LOCK_CHECK_ANSWER, cJSON_CreateTrue());
        ixstb_fe_get_type(fe_id, &type);
        cJSON_AddItemToObject(root, STBD_LOCK_CHECK_FE_TYPE, cJSON_CreateString(ixsys_getstr_fetype(type)));
        cJSON_AddItemToObject(root, STBD_LOCK_CHECK_STRENGTH, cJSON_CreateNumber((pwr * 100) / 0xffff));
        cJSON_AddItemToObject(root, STBD_LOCK_CHECK_SNR, cJSON_CreateNumber((snr * 100) / 0xffff));
        cJSON_AddItemToObject(root, STBD_LOCK_CHECK_BER, cJSON_CreateNumber(ber));
        cJSON_AddItemToObject(root, STBD_LOCK_CHECK_UNC, cJSON_CreateNumber(unc));
    }

    return root;
}

static cJSON* op_proc_motor_move(cJSON *json)
{
    print_here();

    cJSON       *root = cJSON_CreateObject();
    cJSON       *answer;
    char        ipstr[16];
    int         fe_id = cJSON_GetObjectItem(json, HTTPD_MOTOR_MOVE_FE_ID)->valueint;
    int         sat_id = cJSON_GetObjectItem(json, HTTPD_MOTOR_MOVE_SAT_ID)->valueint;
    char        type[8];
    fe_type_t   fe_type;
    ix_usals_cmd_t  usals_cmd;
    int         fd, param[3], n_param;

    ixstb_fe_get_type(fe_id, &fe_type);

    if (fe_type == FE_QPSK) {
        strcpy(type, cJSON_GetObjectItem(json, HTTPD_MOTOR_MOVE_TYPE)->valuestring);

        if (strcmp(type, HTTPD_MOTOR_MOVE_STEP) == 0) {
            usals_cmd = _usals_step;
            param[0] = cJSON_GetObjectItem(json, type)->valueint;
            n_param = 1;
        } else if (strcmp(type, HTTPD_MOTOR_MOVE_ANGLE) == 0) {
            usals_cmd = _usals_angle;
            param[0] = cJSON_GetObjectItem(json, type)->valueint;
            n_param = 1;
        } else if (strcmp(type, HTTPD_MOTOR_MOVE_GOTO_N) == 0) {
            usals_cmd = _usals_goto_n;
            param[0] = cJSON_GetObjectItem(json, type)->valueint;
            n_param = 1;
        } else if (strcmp(type, HTTPD_MOTOR_MOVE_STORE_N) == 0) {
            usals_cmd = _usals_store_n;
            param[0] = cJSON_GetObjectItem(json, type)->valueint;
            n_param = 1;
        } else if (strcmp(type, HTTPD_MOTOR_MOVE_USALS) == 0) {
            usals_cmd = _usals_auto;
            param[0] = cJSON_GetObjectItem(json, type)->valueint;
            param[1] = cJSON_GetObjectItem(json, HTTPD_MOTOR_MOVE_LONGITUDE)->valueint;
            param[2] = cJSON_GetObjectItem(json, HTTPD_MOTOR_MOVE_LATITUDE)->valueint;
            n_param = 3;
        } else {
            usals_cmd = _usals_stop;
            n_param = 0;
        }

        if (ixstb_fe_get_desc(fe_id, &fd) != 0) {
            ix_dvbs_t   dvbs;

            ixsys_get_dvbs(DB_PATH_DATA, sat_id, &dvbs);
            ixstb_fe_try_lock(fe_id, dvbs.key_tp);
            ixstb_fe_get_desc(fe_id, &fd);
        }
        ixstb_fe_dvbs_set_diseqc_motor(fd, usals_cmd, param, n_param);

        if (usals_cmd == _usals_store_n) {
            ix_dvbs_t   dvbs;

            ixsys_get_dvbs(DB_PATH_DATA, sat_id, &dvbs);
            dvbs.diseqc = _diseqc_motor;
            dvbs.diseqc_port = param[0];
            ixsys_update_dvbs(DB_PATH_DATA, &dvbs);
        } else if (usals_cmd == _usals_auto) {
            ixsys_set_user_position(param[1], param[2]);
        }

        answer = cJSON_CreateTrue();
    } else {
        answer = cJSON_CreateFalse();
    }

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_MOTOR_MOVE));
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(root, STBD_MOTOR_MOVE_MY_IP, cJSON_CreateString(ipstr));
    cJSON_AddItemToObject(root, STBD_MOTOR_MOVE_ANSWER, answer);

    return root;
}

static void scan_callback(ix_scan_step_t step, void *param, size_t param_size, void *priv)
{
    int     *done = (int*)priv;

    // print_debug("%s(), %s\n", __FUNCTION__, (char*)param);

    if (step == _scan_step_error) {
        *done = -1;
    } else if (step ==_scan_step_done) {
        int count = *(int*)param;

        if (count > 0)
            *done = 1;
        else
            *done = -1;
    }
}

static cJSON* op_proc_scan(cJSON *json)
{
    print_here();

    int     fe_id = cJSON_GetObjectItem(json, HTTPD_SCAN_FE_ID)->valueint;
    int     rf_id = cJSON_GetObjectItem(json, HTTPD_SCAN_RF_ID)->valueint;
    ix_scan_handle_t    handle;
    ix_scan_param_t     param;
    int     done = 0;
    fe_status_t status;

    if (ixstb_fe_try_lock(fe_id, rf_id) == 0) {
        sleep(2);

        status = ixstb_fe_get_signal_status(fe_id, NULL, NULL, NULL, NULL);
        if (status & FE_HAS_LOCK) {
            param.cb = scan_callback;
            strcpy(param.db_path, DB_PATH_DATA);
            param.priv = (void*)&done;

            handle = ixstb_scan_request(fe_id, rf_id, &param);
            while (!done)
                sleep(1);
            ixstb_scan_retract(handle);

            return (done == 1) ? db_is_ready(param.db_path) : NULL ;
        }
    }

    return NULL;
}

static cJSON* op_proc_channel_list(cJSON *json)
{
    print_here();
    return db_is_ready(DB_PATH_DATA);
}

static cJSON* op_proc_streaming(cJSON *json)
{
    print_here();

    cJSON   *root = cJSON_CreateObject();
    cJSON   *item = cJSON_GetObjectItem(json, HTTPD_STREAMING_STOP);
    char    str[32];
    int     n;
    ix_player_handle_t  handle = NULL;

    if (item) {
        n = ixstb_player_retract((ix_player_handle_t)strtoul(item->valuestring, NULL, 16));
    } else {
        ix_player_param_t   player_param;
        ix_streaming_t      type = ixsys_get_streamingtype_str(cJSON_GetObjectItem(json, HTTPD_STREAMING_TYPE)->valuestring);
        int     port = cJSON_GetObjectItem(json, HTTPD_STREAMING_PORT)->valueint;
        int     ttl = cJSON_GetObjectItem(json, HTTPD_STREAMING_TTL)->valueint;

        memset(&player_param, 0, sizeof(player_param));

        if ((item = cJSON_GetObjectItem(json, HTTPD_STREAMING_PATH))) {
            strcpy(player_param.stream_file.ip, cJSON_GetObjectItem(json, HTTPD_STREAMING_MY_IP)->valuestring);
            player_param.stream_file.type = type;
            player_param.stream_file.port = port;
            player_param.stream_file.ttl  = ttl;
            player_param.stream_file.path = item->valuestring;

            handle = ixstb_player_request(_playtype_streaming_file, &player_param);
        } else {
            strcpy(player_param.stream_file.ip, cJSON_GetObjectItem(json, HTTPD_STREAMING_MY_IP)->valuestring);
            player_param.stream_live.type = type;
            player_param.stream_live.port = port;
            player_param.stream_live.ttl  = ttl;
            player_param.stream_live.fe_id = cJSON_GetObjectItem(json, HTTPD_STREAMING_FE_ID)->valueint;
            player_param.stream_live.ch_id = cJSON_GetObjectItem(json, HTTPD_STREAMING_CH_ID)->valueint;
        }

        handle = ixstb_player_request(_playtype_streaming_live, &player_param);

        n = handle ? 0 : -1 ;
    }

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_STREAMING));
    ixsys_get_ipaddr(NULL, str, sizeof(str));
    cJSON_AddItemToObject(root, STBD_STREAMING_MY_IP, cJSON_CreateString(str));
    cJSON_AddItemToObject(root, STBD_STREAMING_ANSWER, (n == 0) ? cJSON_CreateTrue() : cJSON_CreateFalse());
    snprintf(str, sizeof(str), "%p", handle);
    cJSON_AddItemToObject(root, STBD_STREAMING_HANDLE, cJSON_CreateString(str + 2));

    return root;
}

static cJSON* op_proc_recording(cJSON *json)
{
    print_here();

    cJSON   *root = cJSON_CreateObject();
    cJSON   *item = cJSON_GetObjectItem(json, HTTPD_RECORDING_STOP);
    char    str[32];
    int     n;
    ix_player_handle_t  handle = NULL;

    if (item) {
        n = ixstb_player_retract((ix_player_handle_t)strtol(item->valuestring, NULL, 16));
    } else {
        ix_player_param_t   player_param;

        item = cJSON_GetObjectItem(json, HTTPD_RECORDING_CH_ID);

        memset(&player_param, 0, sizeof(player_param));
        player_param.record_live.fe_id = cJSON_GetObjectItem(json, HTTPD_RECORDING_FE_ID)->valueint;
        player_param.record_live.ch_id = (item) ? item->valueint : 0 ;


        handle = ixstb_player_request(_playtype_recording_live, &player_param);
        n = handle ? 0 : -1 ;
    }

    cJSON_AddItemToObject(root, STBD_OPCODE, cJSON_CreateString(STBD_RECORDING));
    ixsys_get_ipaddr(NULL, str, sizeof(str));
    cJSON_AddItemToObject(root, STBD_RECORDING_MY_IP, cJSON_CreateString(str));
    cJSON_AddItemToObject(root, STBD_RECORDING_ANSWER, (n == 0) ? cJSON_CreateTrue() : cJSON_CreateFalse());
    snprintf(str, sizeof(str), "%p", handle);
    cJSON_AddItemToObject(root, STBD_RECORDING_HANDLE, cJSON_CreateString(str + 2));

    return root;
}

/* ****************************************************************************************************************** */

static ixcgi_opcode_t   opcode_list_[] = {
    {   HTTPD_SYSTEM_INFO,      op_proc_system_info     },
    {   HTTPD_DVBS_LIST,        op_proc_dvbs_list       },
    {   HTTPD_DVBS_TP_LIST,     op_proc_dvbs_tp_list    },
    {   HTTPD_DVBS,             op_proc_dvbs            },
    {   HTTPD_DVBS_TP,          op_proc_dvbs_tp         },
    {   HTTPD_ADD_DVBS,         op_proc_add_dvbs        },
    {   HTTPD_UPDATE_DVBS,      op_proc_update_dvbs     },
    {   HTTPD_DELETE_DVBS,      op_proc_delete_dvbs     },
    {   HTTPD_ADD_DVBS_TP,      op_proc_add_dvbs_tp     },
    {   HTTPD_UPDATE_DVBS_TP,   op_proc_update_dvbs_tp  },
    {   HTTPD_DELETE_DVBS_TP,   op_proc_delete_dvbs_tp  },
    {   HTTPD_TRY_LOCK,         op_proc_try_lock        },
    {   HTTPD_LOCK_CHECK,       op_proc_lock_check      },
    {   HTTPD_MOTOR_MOVE,       op_proc_motor_move      },
    {   HTTPD_SCAN,             op_proc_scan            },
    {   HTTPD_CHANNEL_LIST,     op_proc_channel_list    },
    {   HTTPD_STREAMING,        op_proc_streaming       },
    {   HTTPD_RECORDING,        op_proc_recording       },
};

/* ****************************************************************************************************************** */

char* ixsvc_cmd_parser(char *data, ssize_t len)
{
    cJSON   *json = cJSON_Parse(data);
    cJSON   *t = NULL;
    char    *out = NULL;
    int     n;
    int     loop = sizeof(opcode_list_) / sizeof(ixcgi_opcode_t);

    if (!json) {
        print_error("illegal data");
        return NULL;
    }

    if ((t = cJSON_GetObjectItem(json, HTTPD_OPCODE)) == NULL) {
        print_error("illegal data");
        cJSON_Delete(json);
        return NULL;
    }

    for (n = 0; n < loop; ++n) {
        if (strcmp(opcode_list_[n].op, t->valuestring) == 0) {
            if ((t = (opcode_list_[n].proc)(json)) != NULL) {
                out = cJSON_Print(t);
                cJSON_Delete(t);
            }

            break;
        }
    }

    cJSON_Delete(json);

    return out;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
