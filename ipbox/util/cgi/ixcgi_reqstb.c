/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixcgi_reqstb.h"
#include <sys/un.h>
#include <arpa/inet.h>
#include <string.h>

/* ****************************************************************************************************************** */

static int send_recv_stbd(char *out, char *in, size_t in_size, int timeout)
{
    int     sock;
    ssize_t n_recv = 0;
    fd_set  rset;
    struct sockaddr_un  local_addr, remote_addr;
    struct timeval      tv;

    if (access(STB_TO_CGI_PATH, F_OK) == 0)
        unlink(STB_TO_CGI_PATH);

    if ((sock = socket(PF_FILE, SOCK_DGRAM, 0)) == -1)
        return 0;

    bzero(&local_addr, sizeof(local_addr));
    local_addr.sun_family = AF_FILE;
    strcpy(local_addr.sun_path, STB_TO_CGI_PATH);

    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) == -1) {
        close(sock);
        return 0;
    }

    bzero(&remote_addr, sizeof(remote_addr));
    remote_addr.sun_family = AF_FILE;
    strcpy(remote_addr.sun_path, CGI_TO_STB_PATH);

    sendto(sock, (void*)out, strlen(out), 0, (struct sockaddr*)&remote_addr, sizeof(remote_addr));

    FD_ZERO(&rset);
    FD_SET(sock, &rset);

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    if (select(sock + 1, &rset, NULL, NULL, &tv) > 0)
        n_recv = recvfrom(sock, in, in_size, 0, NULL, NULL);

    close(sock);

    return n_recv;
}

int ixcgi_reqstb_system_info(char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_SYSTEM_INFO));
    cJSON_AddItemToObject(root, HTTPD_SYSTEM_INFO_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 1000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_dvbs_list(char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_DVBS_LIST));
    cJSON_AddItemToObject(root, HTTPD_DVBS_LIST_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_dvbs_tp_list(char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_DVBS_TP_LIST));
    cJSON_AddItemToObject(root, HTTPD_DVBS_TP_LIST_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_dvbs(char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_DVBS));
    cJSON_AddItemToObject(root, HTTPD_DVBS_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_dvbs_tp(char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_DVBS_TP));
    cJSON_AddItemToObject(root, HTTPD_DVBS_TP_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_add_dvbs(ix_dvbs_t *dvbs, char *in, size_t in_size)
{
    cJSON   *root, *item;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_ADD_DVBS));
    cJSON_AddItemToObject(root, HTTPD_ADD_DVBS_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_ADD_DVBS_DVBS, item = cJSON_CreateObject());

    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_NAME, cJSON_CreateString(dvbs->name));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_LNB, cJSON_CreateNumber(dvbs->lnb));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_LNBF_LO, cJSON_CreateNumber(dvbs->lnbf_lo));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_LNBF_HI, cJSON_CreateNumber(dvbs->lnbf_hi));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_LNBF_REF, cJSON_CreateNumber(dvbs->lnbf_ref));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_LNBP, cJSON_CreateNumber(dvbs->lnbp));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_DISEQC, cJSON_CreateNumber(dvbs->diseqc));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_DISEQC_PORT, cJSON_CreateNumber(dvbs->diseqc_port));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_DVBS_SAT_POS, cJSON_CreateNumber(dvbs->sat_pos));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_update_dvbs(ix_dvbs_t *dvbs, char *in, size_t in_size)
{
    cJSON   *root, *item;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_UPDATE_DVBS));
    cJSON_AddItemToObject(root, HTTPD_UPDATE_DVBS_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_UPDATE_DVBS_DVBS, item = cJSON_CreateObject());

    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_SAT_ID, cJSON_CreateNumber(dvbs->sat_id));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_NAME, cJSON_CreateString(dvbs->name));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_LNB, cJSON_CreateNumber(dvbs->lnb));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_LNBF_LO, cJSON_CreateNumber(dvbs->lnbf_lo));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_LNBF_HI, cJSON_CreateNumber(dvbs->lnbf_hi));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_LNBF_REF, cJSON_CreateNumber(dvbs->lnbf_ref));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_LNBP, cJSON_CreateNumber(dvbs->lnbp));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_DISEQC, cJSON_CreateNumber(dvbs->diseqc));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_DISEQC_PORT, cJSON_CreateNumber(dvbs->diseqc_port));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_SAT_POS, cJSON_CreateNumber(dvbs->sat_pos));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_DVBS_KEY_TP, cJSON_CreateNumber(dvbs->key_tp));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_delete_dvbs(int sat_id, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_DELETE_DVBS));
    cJSON_AddItemToObject(root, HTTPD_DELETE_DVBS_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_DELETE_DVBS_SAT_ID, cJSON_CreateNumber(sat_id));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_add_dvbs_tp(ix_dvbs_tp_t *dvbs_tp, char *in, size_t in_size)
{
    cJSON   *root, *item;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_ADD_DVBS_TP));
    cJSON_AddItemToObject(root, HTTPD_ADD_DVBS_TP_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_ADD_DVBS_TP_DVBS_TP, item = cJSON_CreateObject());

    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_FREQ, cJSON_CreateNumber(dvbs_tp->freq));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_SYM, cJSON_CreateNumber(dvbs_tp->sym));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_POLAR, cJSON_CreateNumber(dvbs_tp->polar));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_DELIVERY, cJSON_CreateNumber(dvbs_tp->system));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_MODULATION, cJSON_CreateNumber(dvbs_tp->modulation));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_ORG_NETID, cJSON_CreateNumber(dvbs_tp->org_netid));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_NETID, cJSON_CreateNumber(dvbs_tp->netid));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_TSID, cJSON_CreateNumber(dvbs_tp->tsid));
    cJSON_AddItemToObject(item, HTTPD_ADD_DVBS_TP_DVBS_TP_SAT_ID, cJSON_CreateNumber(dvbs_tp->sat_id));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_update_dvbs_tp(ix_dvbs_tp_t *dvbs_tp, char *in, size_t in_size)
{
    cJSON   *root, *item;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_UPDATE_DVBS_TP));
    cJSON_AddItemToObject(root, HTTPD_UPDATE_DVBS_TP_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_UPDATE_DVBS_TP_DVBS_TP, item = cJSON_CreateObject());

    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_RF_ID, cJSON_CreateNumber(dvbs_tp->rf_id));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_FREQ, cJSON_CreateNumber(dvbs_tp->freq));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_SYM, cJSON_CreateNumber(dvbs_tp->sym));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_POLAR, cJSON_CreateNumber(dvbs_tp->polar));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_DELIVERY, cJSON_CreateNumber(dvbs_tp->system));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_MODULATION, cJSON_CreateNumber(dvbs_tp->modulation));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_ORG_NETID, cJSON_CreateNumber(dvbs_tp->org_netid));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_NETID, cJSON_CreateNumber(dvbs_tp->netid));
    cJSON_AddItemToObject(item, HTTPD_UPDATE_DVBS_TP_DVBS_TP_TSID, cJSON_CreateNumber(dvbs_tp->tsid));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_delete_dvbs_tp(int rf_id, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_DELETE_DVBS_TP));
    cJSON_AddItemToObject(root, HTTPD_DELETE_DVBS_TP_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_DELETE_DVBS_TP_RF_ID, cJSON_CreateNumber(rf_id));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_try_lock(int fe_id, int rf_id, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_TRY_LOCK));
    cJSON_AddItemToObject(root, HTTPD_TRY_LOCK_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_TRY_LOCK_FE_ID, cJSON_CreateNumber(fe_id));
    cJSON_AddItemToObject(root, HTTPD_TRY_LOCK_RF_ID, cJSON_CreateNumber(rf_id));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 5000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_lock_check(int fe_id, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_LOCK_CHECK));
    cJSON_AddItemToObject(root, HTTPD_LOCK_CHECK_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_LOCK_CHECK_FE_ID, cJSON_CreateNumber(fe_id));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 5000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_move_to(int fe_id, int sat_id, char *type, char *param, char *longitude, char *latitude, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;
    int     where;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_MOTOR_MOVE));
    cJSON_AddItemToObject(root, HTTPD_MOTOR_MOVE_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_MOTOR_MOVE_FE_ID, cJSON_CreateNumber(fe_id));
    cJSON_AddItemToObject(root, HTTPD_MOTOR_MOVE_SAT_ID, cJSON_CreateNumber(sat_id));
    cJSON_AddItemToObject(root, HTTPD_MOTOR_MOVE_TYPE, cJSON_CreateString(type));

    if (strcmp(type, HTTPD_MOTOR_MOVE_STEP) == 0) {
        where = atoi(param + 1);
        if (param[0] == 'W')
            where = 3600 - where;
        cJSON_AddItemToObject(root, type, cJSON_CreateNumber(where));
    } else if (strcmp(type, HTTPD_MOTOR_MOVE_ANGLE) == 0) {
        where = atoi(param + 1);
        if (param[0] == 'W')
            where = 3600 - where;
        cJSON_AddItemToObject(root, type, cJSON_CreateNumber(where));
    } else if (strcmp(type, HTTPD_MOTOR_MOVE_GOTO_N) == 0) {
        where = atoi(param);
        cJSON_AddItemToObject(root, type, cJSON_CreateNumber(where));
    } else if (strcmp(type, HTTPD_MOTOR_MOVE_STORE_N) == 0) {
        where = atoi(param);
        cJSON_AddItemToObject(root, type, cJSON_CreateNumber(where));
    } else if (strcmp(type, HTTPD_MOTOR_MOVE_USALS) == 0) {
        where = atoi(param + 1);
        if (param[0] == 'W')
            where = 3600 - where;
        cJSON_AddItemToObject(root, type, cJSON_CreateNumber(where));
        where = atoi(longitude + 1);
        if (longitude[0] == 'W')
            where = 3600 - where;
        cJSON_AddItemToObject(root, HTTPD_MOTOR_MOVE_LONGITUDE, cJSON_CreateNumber(where));
        where = atoi(latitude + 1);
        if (latitude[0] == 'S')
            where = 3600 - where;;
        cJSON_AddItemToObject(root, HTTPD_MOTOR_MOVE_LATITUDE, cJSON_CreateNumber(where));
    }

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 3000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_scan(int fe_id, int rf_id, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_SCAN));
    cJSON_AddItemToObject(root, HTTPD_SCAN_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));
    cJSON_AddItemToObject(root, HTTPD_SCAN_FE_ID, cJSON_CreateNumber(fe_id));
    cJSON_AddItemToObject(root, HTTPD_SCAN_RF_ID, cJSON_CreateNumber(rf_id));

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 20000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_channel_list(int fe_id, int rf_id, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_CHANNEL_LIST));
    cJSON_AddItemToObject(root, HTTPD_CHANNEL_LIST_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));

    if (fe_id >= 0) {
        cJSON_AddItemToObject(root, HTTPD_CHANNEL_LIST_FE_ID, cJSON_CreateNumber(fe_id));
        if (rf_id > 0)
            cJSON_AddItemToObject(root, HTTPD_CHANNEL_LIST_RF_ID, cJSON_CreateNumber(rf_id));
    }

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_streaming(char *handle, char *type, int16_t port, int16_t ttl, int fe_id, int ch_id, char *path, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;
    int     timeout = 12000;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_STREAMING));
    cJSON_AddItemToObject(root, HTTPD_STREAMING_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));

    if (handle[0]) {
        cJSON_AddItemToObject(root, HTTPD_STREAMING_STOP, cJSON_CreateString(handle));
        timeout = 2000;
    } else {
        cJSON_AddItemToObject(root, HTTPD_STREAMING_TYPE, cJSON_CreateString(type));
        cJSON_AddItemToObject(root, HTTPD_STREAMING_PORT, cJSON_CreateNumber(port));
        cJSON_AddItemToObject(root, HTTPD_STREAMING_TTL, cJSON_CreateNumber(ttl));

        if ((fe_id >= 0) && (ch_id > 0)) {
            cJSON_AddItemToObject(root, HTTPD_STREAMING_FE_ID, cJSON_CreateNumber(fe_id));
            cJSON_AddItemToObject(root, HTTPD_STREAMING_CH_ID, cJSON_CreateNumber(ch_id));
        } else {
            cJSON_AddItemToObject(root, HTTPD_STREAMING_PATH, cJSON_CreateString(path));
        }
    }

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, timeout);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

int ixcgi_reqstb_recording(char *handle, int fe_id, int ch_id, char *in, size_t in_size)
{
    cJSON   *root;
    char    *out;
    size_t  n_recv;

    if ((root = cJSON_CreateObject()) == NULL)
        return -1;

    cJSON_AddItemToObject(root, HTTPD_OPCODE, cJSON_CreateString(HTTPD_RECORDING));
    cJSON_AddItemToObject(root, HTTPD_RECORDING_MY_IP, cJSON_CreateString(getenv("REMOTE_ADDR")));

    if (handle[0]) {
        cJSON_AddItemToObject(root, HTTPD_RECORDING_STOP, cJSON_CreateString(handle));
    } else {
        cJSON_AddItemToObject(root, HTTPD_RECORDING_FE_ID, cJSON_CreateNumber(fe_id));
        if (ch_id > 0)
            cJSON_AddItemToObject(root, HTTPD_RECORDING_CH_ID, cJSON_CreateNumber(ch_id));
    }

    n_recv = send_recv_stbd(out = cJSON_Print(root), in, in_size, 2000);

    cJSON_Delete(root);
    free(out);

    return (n_recv == 0) ? -1 : 0 ;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
