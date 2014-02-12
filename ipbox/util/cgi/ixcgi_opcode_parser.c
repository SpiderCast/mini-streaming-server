/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixcgi_opcode_parser.h"
#include <string.h>
#include "ixcgi_reqstb.h"
#include "ixcgi_makepage.h"
#include "ixcgi_sql.h"
#include "ixcgi_getenv.h"
#include "ixsys_getstr.h"

/* ****************************************************************************************************************** */

enum { RECV_BUF_SIZE = 2048 };

/* ****************************************************************************************************************** */

static cJSON* op_proc_system_info()
{
    char    in[RECV_BUF_SIZE];

    if (ixcgi_reqstb_system_info(in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_dvbs_list()
{
    char    in[RECV_BUF_SIZE];
    cJSON   *root;
    char    path[128];

    if (ixcgi_reqstb_dvbs_list(in, RECV_BUF_SIZE) == 0) {
        if ((root = cJSON_Parse(in)) != NULL) {
            if (strcmp(cJSON_GetObjectItem(root, STBD_OPCODE)->valuestring, STBD_DB_READY) == 0) {
                if (cJSON_GetObjectItem(root, STBD_DB_READY_ANSWER)->type == cJSON_True) {
                    strcpy(path, cJSON_GetObjectItem(root, STBD_DB_READY_PATH)->valuestring);
                    cJSON_Delete(root);
                    return ixcgi_sql_dvbs_list(path);
                }
            }

            cJSON_Delete(root);
        }
    }

    return ixcgi_make_error_page(_error_db_busy);
}

static cJSON* op_proc_dvbs_tp_list()
{
    char    in[RECV_BUF_SIZE];
    cJSON   *root;
    char    *token = ixcgi_get_query_string(HTTPD_DVBS_TP_LIST_SAT_ID);
    int     sat_id = 0;
    char    path[128];

    if (token) {
        sat_id = atoi(token);
        free(token);
    }

    if (ixcgi_reqstb_dvbs_tp_list(in, RECV_BUF_SIZE) == 0) {
        if ((root = cJSON_Parse(in)) != NULL) {
            if (strcmp(cJSON_GetObjectItem(root, STBD_OPCODE)->valuestring, STBD_DB_READY) == 0) {
                if (cJSON_GetObjectItem(root, STBD_DB_READY_ANSWER)->type == cJSON_True) {
                    strcpy(path, cJSON_GetObjectItem(root, STBD_DB_READY_PATH)->valuestring);
                    cJSON_Delete(root);
                    return ixcgi_sql_dvbs_tp_list(path, sat_id);
                }
            }

            cJSON_Delete(root);
        }
    }

    return ixcgi_make_error_page(_error_db_busy);
}

static cJSON* op_proc_dvbs()
{
    char    in[RECV_BUF_SIZE];
    cJSON   *root;
    char    *token = ixcgi_get_query_string(HTTPD_DVBS_SAT_ID);
    int     sat_id;
    char    path[128];

    if (!token) {
        return ixcgi_make_error_page(_error_illegal_data);
    }

    sat_id = atoi(token);
    free(token);

    if (ixcgi_reqstb_dvbs(in, RECV_BUF_SIZE) == 0) {
        if ((root = cJSON_Parse(in)) != NULL) {
            if (strcmp(cJSON_GetObjectItem(root, STBD_OPCODE)->valuestring, STBD_DB_READY) == 0) {
                if (cJSON_GetObjectItem(root, STBD_DB_READY_ANSWER)->type == cJSON_True) {
                    strcpy(path, cJSON_GetObjectItem(root, STBD_DB_READY_PATH)->valuestring);
                    cJSON_Delete(root);
                    return ixcgi_sql_dvbs(path, sat_id);
                }
            }

            cJSON_Delete(root);
        }
    }

    return ixcgi_make_error_page(_error_db_busy);
}

static cJSON* op_proc_dvbs_tp()
{
    char    in[RECV_BUF_SIZE];
    cJSON   *root;
    char    *token = ixcgi_get_query_string(HTTPD_DVBS_TP_RF_ID);
    int     rf_id;
    char    path[128];

    if (!token) {
        return ixcgi_make_error_page(_error_illegal_data);
    }

    rf_id = atoi(token);
    free(token);

    if (ixcgi_reqstb_dvbs_tp(in, RECV_BUF_SIZE) == 0) {
        if ((root = cJSON_Parse(in)) != NULL) {
            if (strcmp(cJSON_GetObjectItem(root, STBD_OPCODE)->valuestring, STBD_DB_READY) == 0) {
                if (cJSON_GetObjectItem(root, STBD_DB_READY_ANSWER)->type == cJSON_True) {
                    strcpy(path, cJSON_GetObjectItem(root, STBD_DB_READY_PATH)->valuestring);
                    cJSON_Delete(root);
                    return ixcgi_sql_dvbs_tp(path, rf_id);
                }
            }
        }
    }

    return ixcgi_make_error_page(_error_db_busy);
}

static cJSON* op_proc_add_dvbs()
{
    char    in[RECV_BUF_SIZE];
    char    *token = NULL;
    ix_dvbs_t   dvbs;

    memset(&dvbs, 0, sizeof(dvbs));

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_NAME)) == NULL) {
        return ixcgi_make_error_page(_error_illegal_data);
    }
    strcpy(dvbs.name, token);
    free(token);

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_LNB))) {
        dvbs.lnb = ixsys_get_lnb_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_LNBF_LO))) {
        dvbs.lnbf_lo = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_LNBF_HI))) {
        dvbs.lnbf_hi = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_LNBF_REF))) {
        dvbs.lnbf_ref = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_LNBP))) {
        dvbs.lnbp = ixsys_get_lnbp_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_22KHZ))) {
        dvbs._22khz = ixsys_get_22khz_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_DISEQC))) {
        dvbs.diseqc = ixsys_get_diseqc_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_DISEQC_PORT))) {
        dvbs.diseqc_port = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_DVBS_SAT_POS))) {
        dvbs.sat_pos = atoi(token);
        free(token);
    }

    if (ixcgi_reqstb_add_dvbs(&dvbs, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_update_dvbs()
{
    char    in[RECV_BUF_SIZE];
    char    *token = NULL;
    ix_dvbs_t   dvbs;

    memset(&dvbs, 0, sizeof(dvbs));

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_SAT_ID))) {
        dvbs.sat_id = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_NAME)) == NULL) {
        return ixcgi_make_error_page(_error_illegal_data);
    }
    strcpy(dvbs.name, token);
    free(token);

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_LNB))) {
        dvbs.lnb = ixsys_get_lnb_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_LNBF_LO))) {
        dvbs.lnbf_lo = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_LNBF_HI))) {
        dvbs.lnbf_hi = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_LNBF_REF))) {
        dvbs.lnbf_ref = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_LNBP))) {
        dvbs.lnbp = ixsys_get_lnbp_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_22KHZ))) {
        dvbs._22khz = ixsys_get_22khz_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_DISEQC))) {
        dvbs.diseqc = ixsys_get_diseqc_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_DISEQC_PORT))) {
        dvbs.diseqc_port = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_SAT_POS))) {
        dvbs.sat_pos = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_DVBS_KEY_TP))) {
        dvbs.key_tp = atoi(token);
        free(token);
    }

    if (ixcgi_reqstb_update_dvbs(&dvbs, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_delete_dvbs()
{
    char    in[RECV_BUF_SIZE];
    char    *token = ixcgi_get_query_string(HTTPD_DELETE_DVBS_SAT_ID);
    int     sat_id;

    if (!token) {
        return ixcgi_make_error_page(_error_illegal_data);
    }

    sat_id = atoi(token);
    free(token);

    if (ixcgi_reqstb_delete_dvbs(sat_id, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_add_dvbs_tp()
{
    char    in[RECV_BUF_SIZE];
    char    *token = NULL;
    ix_dvbs_tp_t    dvbs_tp;

    memset(&dvbs_tp, 0, sizeof(dvbs_tp));

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_FREQ))) {
        dvbs_tp.freq = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_SYM))) {
        dvbs_tp.sym = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_POLAR))) {
        dvbs_tp.polar = ixsys_get_polar_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_DELIVERY))) {
        dvbs_tp.system = ixsys_get_delivery_system_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_MODULATION))) {
        dvbs_tp.modulation = ixsys_get_modulation_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_ORG_NETID))) {
        dvbs_tp.org_netid = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_NETID))) {
        dvbs_tp.netid = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_TSID))) {
        dvbs_tp.tsid = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_ADD_DVBS_TP_DVBS_TP_SAT_ID))) {
        dvbs_tp.sat_id = atoi(token);
        free(token);
    }

    if (ixcgi_reqstb_add_dvbs_tp(&dvbs_tp, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_update_dvbs_tp()
{
    char    in[RECV_BUF_SIZE];
    char    *token = NULL;
    ix_dvbs_tp_t    dvbs_tp;

    memset(&dvbs_tp, 0, sizeof(dvbs_tp));

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_RF_ID))) {
        dvbs_tp.rf_id = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_TP_FREQ))) {
        dvbs_tp.freq = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_TP_SYM))) {
        dvbs_tp.sym = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_TP_POLAR))) {
        dvbs_tp.polar = ixsys_get_polar_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_TP_DELIVERY))) {
        dvbs_tp.system = ixsys_get_delivery_system_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_TP_MODULATION))) {
        dvbs_tp.modulation = ixsys_get_modulation_str(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_TP_ORG_NETID))) {
        dvbs_tp.org_netid = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_TP_NETID))) {
        dvbs_tp.netid = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_UPDATE_DVBS_TP_DVBS_TP_TSID))) {
        dvbs_tp.tsid = atoi(token);
        free(token);
    }

    if (ixcgi_reqstb_update_dvbs_tp(&dvbs_tp, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_delete_dvbs_tp()
{
    char    in[RECV_BUF_SIZE];
    char    *token = ixcgi_get_query_string(HTTPD_DELETE_DVBS_TP_RF_ID);
    int     rf_id;

    if (!token) {
        return ixcgi_make_error_page(_error_illegal_data);
    }

    rf_id = atoi(token);
    free(token);

    if (ixcgi_reqstb_delete_dvbs_tp(rf_id, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_try_lock()
{
    char    in[RECV_BUF_SIZE];
    char    *token_feid = ixcgi_get_query_string(HTTPD_TRY_LOCK_FE_ID);
    char    *token_rfid = ixcgi_get_query_string(HTTPD_TRY_LOCK_RF_ID);
    int     fe_id;
    int     rf_id;

    if (!token_feid || !token_rfid) {
        if (token_feid) free(token_feid);
        if (token_rfid) free(token_rfid);

        return ixcgi_make_error_page(_error_illegal_data);
    }

    fe_id = atoi(token_feid);
    free(token_feid);

    rf_id = atoi(token_rfid);
    free(token_rfid);

    if (ixcgi_reqstb_try_lock(fe_id, rf_id, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_lock_check()
{
    char    in[RECV_BUF_SIZE];
    char    *token = ixcgi_get_query_string(HTTPD_TRY_LOCK_FE_ID);
    int     fe_id;

    if (!token) {
        return ixcgi_make_error_page(_error_illegal_data);
    }

    fe_id = atoi(token);
    free(token);

    if (ixcgi_reqstb_lock_check(fe_id, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_motor_move()
{
    char    in[RECV_BUF_SIZE];
    char    *token_feid = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_FE_ID);
    char    *token_satid = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_SAT_ID);
    char    *token_type = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_TYPE);
    int     fe_id, sat_id;
    char    type[8];
    char    *longitude, *latitude, *param;

    if (!token_feid || !token_satid || !token_type) {
        if (token_feid)     free(token_feid);
        if (token_satid)    free(token_satid);
        if (token_type)     free(token_type);

        return ixcgi_make_error_page(_error_illegal_data);
    }

    fe_id = atoi(token_feid);
    sat_id = atoi(token_satid);
    strcpy(type, token_type);

    free(token_feid);
    free(token_satid);
    free(token_type);

    longitude = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_LONGITUDE);
    latitude = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_LATITUDE);

    if ((param = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_STEP)) == NULL) {
        if ((param = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_ANGLE)) == NULL) {
            if ((param = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_GOTO_N)) == NULL) {
                if ((param = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_STORE_N)) == NULL) {
                    param = ixcgi_get_query_string(HTTPD_MOTOR_MOVE_USALS);
                }
            }
        }
    }

    if (ixcgi_reqstb_move_to(fe_id, sat_id, type, param, longitude, latitude, in, RECV_BUF_SIZE) == 0) {
        return cJSON_Parse(in);
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_scan()
{
    char    in[RECV_BUF_SIZE];
    char    *token_feid = ixcgi_get_query_string(HTTPD_SCAN_FE_ID);
    char    *token_rfid = ixcgi_get_query_string(HTTPD_SCAN_RF_ID);
    int     fe_id;
    int     rf_id;
    cJSON   *root;
    char    path[128];

    if (!token_feid || !token_rfid) {
        if (token_feid) free(token_feid);
        if (token_rfid) free(token_rfid);

        return ixcgi_make_error_page(_error_illegal_data);
    }

    fe_id = atoi(token_feid);
    free(token_feid);

    rf_id = atoi(token_rfid);
    free(token_rfid);

    if (ixcgi_reqstb_scan(fe_id, rf_id, in, RECV_BUF_SIZE) == 0) {
        if ((root = cJSON_Parse(in)) != NULL) {
            if (strcmp(cJSON_GetObjectItem(root, STBD_OPCODE)->valuestring, STBD_DB_READY) == 0) {
                if (cJSON_GetObjectItem(root, STBD_DB_READY_ANSWER)->type == cJSON_True) {
                    strcpy(path, cJSON_GetObjectItem(root, STBD_DB_READY_PATH)->valuestring);
                    cJSON_Delete(root);
                    return ixcgi_sql_channel_list(path, fe_id, rf_id);
                }
            }

            cJSON_Delete(root);
        }
    }

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_channel_list()
{
    char    in[RECV_BUF_SIZE];
    cJSON   *root, *item;
    char    *token;
    char    path[128];
    int     fe_xx = -1, rf_id = -1;

    memset(path, 0, sizeof(path));

    if ((token = ixcgi_get_query_string(HTTPD_CHANNEL_LIST_FE_ID))) {
        fe_xx = atoi(token);
        free(token);
    }

    if ((token = ixcgi_get_query_string(HTTPD_CHANNEL_LIST_RF_ID))) {
        rf_id = atoi(token);
        free(token);
    }

    if (ixcgi_reqstb_channel_list(fe_xx, rf_id, in, RECV_BUF_SIZE) == 0) {
        if ((root = cJSON_Parse(in)) != NULL) {
            if (strcmp(cJSON_GetObjectItem(root, STBD_OPCODE)->valuestring, STBD_DB_READY) == 0) {
                if (cJSON_GetObjectItem(root, STBD_DB_READY_ANSWER)->type == cJSON_True) {
                    strcpy(path, cJSON_GetObjectItem(root, STBD_DB_READY_PATH)->valuestring);

                    if ((item = cJSON_GetObjectItem(root, STBD_CHANNEL_LIST_FE_TYPE))) {
                        fe_xx = item->valueint;
                        if ((item = cJSON_GetObjectItem(root, STBD_CHANNEL_LIST_RF_ID)))
                            rf_id = item->valueint;
                    }

                    cJSON_Delete(root);
                    return ixcgi_sql_channel_list(path, fe_xx, rf_id);
                }
            }

            cJSON_Delete(root);
        }
    }

    return ixcgi_make_error_page(_error_db_busy);
}

static cJSON* op_proc_streaming()
{
    char    in[RECV_BUF_SIZE];
    char    *token;
    char    handle[32] = { 0 };
    int16_t port = 0, ttl = 0;
    char    type[12], path[128];
    int     fe_id = -1, ch_id = -1;

    memset(type, 0, sizeof(type));
    memset(path, 0, sizeof(path));

    if ((token = ixcgi_get_query_string(HTTPD_STREAMING_STOP))) {
        strcpy(handle, token);
        free(token);
    } else {
        if ((token = ixcgi_get_query_string(HTTPD_STREAMING_TYPE))) {
            strcpy(type, token);
            free(token);
        }

        if ((token = ixcgi_get_query_string(HTTPD_STREAMING_PORT))) {
            port = atoi(token);
            free(token);
        }

        if ((token = ixcgi_get_query_string(HTTPD_STREAMING_TTL))) {
            ttl = atoi(token);
            free(token);
        }

        if ((token = ixcgi_get_query_string(HTTPD_STREAMING_FE_ID))) {
            fe_id = atoi(token);
            free(token);
        }

        if ((token = ixcgi_get_query_string(HTTPD_STREAMING_CH_ID))) {
            ch_id = atoi(token);
            free(token);
        }

        if ((token = ixcgi_get_query_string(HTTPD_STREAMING_PATH))) {
            snprintf(path, sizeof(path), "%s", token);
            free(token);
        }
    }

    if (ixcgi_reqstb_streaming(handle, type, port, ttl, fe_id, ch_id, path, in, RECV_BUF_SIZE) == 0)
        return cJSON_Parse(in);

    return ixcgi_make_error_page(_error_stbd_timeout);
}

static cJSON* op_proc_recording()
{
    char    in[RECV_BUF_SIZE];
    char    *token;
    char    handle[32] = { 0 };
    int     fe_id = -1, ch_id = -1;

    if ((token = ixcgi_get_query_string(HTTPD_RECORDING_STOP))) {
        strcpy(handle, token);
        free(token);
    } else {
        if ((token = ixcgi_get_query_string(HTTPD_RECORDING_FE_ID))) {
            fe_id = atoi(token);
            free(token);
        }

        if ((token = ixcgi_get_query_string(HTTPD_RECORDING_CH_ID))) {
            ch_id = atoi(token);
            free(token);
        }
    }

    if (ixcgi_reqstb_recording(handle, fe_id, ch_id, in, RECV_BUF_SIZE) == 0)
        return cJSON_Parse(in);

    return ixcgi_make_error_page(_error_stbd_timeout);
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

cJSON* ixcgi_opcode_parser(char *value)
{
    int     n;
    int     loop = sizeof(opcode_list_) / sizeof(ixcgi_opcode_t);

    for (n = 0; n < loop; ++n) {
        if (strcmp(opcode_list_[n].op, value) == 0) {
            return (opcode_list_[n].proc)();
        }
    }

    return NULL;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
