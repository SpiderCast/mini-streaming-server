/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IX_JSONOP_H_
#define IX_JSONOP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include <cJSON/cJSON.h>

/* ****************************************************************************************************************** */

enum { OPCODE_STRLEN = 24 };

typedef struct _ixcgi_opcode_t {
    char    op[OPCODE_STRLEN];
    cJSON*  (*proc)();
} ixcgi_opcode_t;

/* ****************************************************************************************************************** */

#define JOP_OPCODE              "opcode"

#define JOP_SYSTEM_INFO         "system_info"
#define JOP_DVBS_LIST           "dvbs_list"
#define JOP_DVBS_TP_LIST        "dvbs_tp_list"
#define JOP_CHANNEL_LIST        "channel_list"
#define JOP_ADD_DVBS            "add_dvbs"
#define JOP_UPDATE_DVBS         "update_dvbs"
#define JOP_DELETE_DVBS         "delete_dvbs"
#define JOP_ADD_DVBS_TP         "add_dvbs_tp"
#define JOP_UPDATE_DVBS_TP      "update_dvbs_tp"
#define JOP_DELETE_DVBS_TP      "delete_dvbs_tp"
#define JOP_TRY_LOCK            "try_lock"
#define JOP_LOCK_CHECK          "lock_check"
#define JOP_MOTOR_MOVE          "motor_move"
#define JOP_SCAN                "scan"
#define JOP_STREAMING           "streaming"
#define JOP_RECORDING           "recording"

#define JOP_ANGLE               "angle"
#define JOP_ANSWER              "answer"
#define JOP_BW                  "bandwidth"
#define JOP_BER                 "ber"
#define JOP_CHANNEL             "channel"
#define JOP_CH_ID               "ch_id"
#define JOP_COUNT               "count"
#define JOP_DB                  "db"
#define JOP_DB_READY            "db_ready"
#define JOP_DISEQC              "diseqc"
#define JOP_DISEQC_PORT         "diseqc_port"
#define JOP_DVBS                "dvbs"
#define JOP_DVBS_TP             "dvbs_tp"
#define JOP_FE_ID               "fe_id"
#define JOP_FE_LIST             "fe_list"
#define JOP_FREQ                "freq"
#define JOP_GOTO                "goto"
#define JOP_HANDLE              "handle"
#define JOP_HAS_CACH            "has_cach"
#define JOP_HW_VER              "hw_ver"
#define JOP_KEY_TP              "key_tp"
#define JOP_LATITUDE            "latitude"
#define JOP_LNB                 "lnb"
#define JOP_LNBF_HI             "lnbf_hi"
#define JOP_LNBF_LO             "lnbf_lo"
#define JOP_LNBF_REF            "lnbf_ref"
#define JOP_LNBP                "lnbp"
#define JOP_LONGITUDE           "longitude"
#define JOP_MODEL_NAME          "model_name"
#define JOP_MODULATION          "modulation"
#define JOP_MULTICAST           "multicast"
#define JOP_MY_IP               "my_ip"
#define JOP_NAME                "name"
#define JOP_NETID               "netid"
#define JOP_ORG_NETID           "org_netid"
#define JOP_PATH                "path"
#define JOP_POLAR               "polar"
#define JOP_PORT                "port"
#define JOP_RF_ID               "rf_id"
#define JOP_SAT_ID              "sat_id"
#define JOP_SAT_POS             "sat_pos"
#define JOP_SNR                 "snr"
#define JOP_STEP                "step"
#define JOP_STOP                "stop"
#define JOP_STORE               "store"
#define JOP_STRENGTH            "strength"
#define JOP_SW_VER              "sw_ver"
#define JOP_SYM                 "sym"
#define JOP_TSID                "tsid"
#define JOP_TTL                 "ttl"
#define JOP_TYPE                "type"
#define JOP_UNC                 "unc"
#define JOP_USALS               "usals"
#define JOP_VERSION             "version"
#define JOP_22KHZ               "_22khz"
#define JOP_DELIVERY            "delivery"

#define HTTPD_OPCODE            JOP_OPCODE
#define STBD_OPCODE             JOP_OPCODE
#define HTTPD_MY_IP             JOP_MY_IP
#define STBD_MY_IP              JOP_MY_IP

/* first connection - */
#define MOBI_WHERE_ARE_YOU                  "whereau"
# define MOBI_WHERE_ARE_YOU_MY_IP           JOP_MY_IP
#define BOX_IM_HERE                         "imhere"
# define BOX_IM_HERE_MY_IP                  JOP_MY_IP

/* general error */
#define ERROR_CODE                          "error"
# define ERROR_CODE_TIMEOUT                 "timeout"
# define ERROR_CODE_DB_BUSY                 "db_busy"
# define ERROR_CODE_ILLEGAL                 "illegal"
# define ERROR_CODE_UNKNOWN                 "unknown"
# define ERROR_CODE_MY_IP                   JOP_MY_IP

/* "?opcode=system_info" */
#define HTTPD_SYSTEM_INFO                   JOP_SYSTEM_INFO
# define HTTPD_SYSTEM_INFO_MY_IP            HTTPD_MY_IP
#define STBD_SYSTEM_INFO                    JOP_SYSTEM_INFO
# define STBD_SYSTEM_INFO_MY_IP             STBD_MY_IP
# define STBD_SYSTEM_INFO_MODEL_NAME        JOP_MODEL_NAME
# define STBD_SYSTEM_INFO_HW_VER            JOP_HW_VER
# define STBD_SYSTEM_INFO_SW_VER            JOP_SW_VER
# define STBD_SYSTEM_INFO_FE_COUNT          JOP_COUNT
# define STBD_SYSTEM_INFO_FE_LIST           JOP_FE_LIST
#  define STB_SYSTEM_INFO_FE_ID             JOP_FE_ID
#  define STBD_SYSTEM_INFO_FE_PATH          JOP_PATH
#  define STBD_SYSTEM_INFO_FE_TYPE          JOP_TYPE
#  define STBD_SYSTEM_INFO_FE_MODEL_NAME    JOP_MODEL_NAME
# define STBD_SYSTEM_INFO_DB                JOP_DB
#  define STBD_SYSTEM_INFO_DB_VERSION       JOP_VERSION

#define STBD_DB_READY                       JOP_DB_READY
# define STBD_DB_READY_MY_IP                STBD_MY_IP
# define STBD_DB_READY_ANSWER               JOP_ANSWER
# define STBD_DB_READY_PATH                 JOP_PATH

/* "?opcode=dvbs_list" */
#define HTTPD_DVBS_LIST                     JOP_DVBS_LIST
# define HTTPD_DVBS_LIST_MY_IP              HTTPD_MY_IP
#define STBD_DVBS_LIST                      JOP_DVBS_LIST
# define STBD_DVBS_LIST_MY_IP               STBD_MY_IP
# define STBD_DVBS_LIST_COUNT               JOP_COUNT
# define STBD_DVBS_LIST_DVBS                JOP_DVBS
// array of "ix_dvbs_t"

/* "?opcode=dvbs_tp_list&sat_id=0": sat_id=0:all */
#define HTTPD_DVBS_TP_LIST                  JOP_DVBS_TP_LIST
# define HTTPD_DVBS_TP_LIST_MY_IP           HTTPD_MY_IP
# define HTTPD_DVBS_TP_LIST_SAT_ID          JOP_SAT_ID
#define STBD_DVBS_TP_LIST                   JOP_DVBS_TP_LIST
# define STBD_DVBS_TP_LIST_MY_IP            STBD_MY_IP
# define STBD_DVBS_TP_LIST_COUNT            JOP_COUNT
# define STBD_DVBS_TP_LIST_DVBS_TP          JOP_DVBS_TP
// array of "ix_dvbs_tp_t"

/* "?opcode=dvbs&sat_id=1" */
#define HTTPD_DVBS                          JOP_DVBS
# define HTTPD_DVBS_MY_IP                   JOP_MY_IP
# define HTTPD_DVBS_SAT_ID                  JOP_SAT_ID
#define STBD_DVBS                           JOP_DVBS
# define STBD_DVBS_MY_IP                    JOP_MY_IP
# define STBD_DVBS_DVBS                     JOP_DVBS

/* "?opcode=dvbs_tp&rf_id=1" */
#define HTTPD_DVBS_TP                       JOP_DVBS_TP
# define HTTPD_DVBS_TP_MY_IP                JOP_MY_IP
# define HTTPD_DVBS_TP_RF_ID                JOP_RF_ID
#define STBD_DVBS_TP                        JOP_DVBS_TP
# define STBD_DVBS_TP_MY_IP                 JOP_MY_IP
# define STBD_DVBS_TP_DVBS_TP               JOP_DVBS_TP

/* "?opcode=add_dvbs&name=usersat&lnb=universal&lnbf_lo=5150&lnbf_hi=0&lnbf_ref=0&lnbp=on&_22khz=auto&diseqc=10&diseqc_port=2&sat_pos=1030" */
#define HTTPD_ADD_DVBS                      JOP_ADD_DVBS
# define HTTPD_ADD_DVBS_MY_IP               HTTPD_MY_IP
# define HTTPD_ADD_DVBS_DVBS                JOP_DVBS
#  define HTTPD_ADD_DVBS_DVBS_NAME          JOP_NAME
#  define HTTPD_ADD_DVBS_DVBS_LNB           JOP_LNB
#  define HTTPD_ADD_DVBS_DVBS_LNBF_LO       JOP_LNBF_LO
#  define HTTPD_ADD_DVBS_DVBS_LNBF_HI       JOP_LNBF_HI
#  define HTTPD_ADD_DVBS_DVBS_LNBF_REF      JOP_LNBF_REF
#  define HTTPD_ADD_DVBS_DVBS_LNBP          JOP_LNBP
#  define HTTPD_ADD_DVBS_DVBS_22KHZ         JOP_22KHZ
#  define HTTPD_ADD_DVBS_DVBS_DISEQC        JOP_DISEQC
#  define HTTPD_ADD_DVBS_DVBS_DISEQC_PORT   JOP_DISEQC_PORT
#  define HTTPD_ADD_DVBS_DVBS_SAT_POS       JOP_SAT_POS
#define STBD_ADD_DVBS                       JOP_ADD_DVBS
# define STBD_ADD_DVBS_MY_IP                STBD_MY_IP
# define STBD_ADD_DVBS_ANSWER               JOP_ANSWER
# define STBD_ADD_DVBS_SAT_ID               JOP_SAT_ID

/* "?opcode=update_dvbs&sat_id=2&name=usersat&lnb=universal&lnbf_lo=5150&lnbf_hi=0&lnbf_ref=0&lnbp=on&_22khz=auto&diseqc=none&diseqc_port=2&sat_pos=1030&key_tp=4" */
#define HTTPD_UPDATE_DVBS                   JOP_UPDATE_DVBS
# define HTTPD_UPDATE_DVBS_MY_IP            HTTPD_MY_IP
# define HTTPD_UPDATE_DVBS_DVBS             JOP_DVBS
#  define HTTPD_UPDATE_DVBS_DVBS_SAT_ID     JOP_SAT_ID
#  define HTTPD_UPDATE_DVBS_DVBS_NAME       JOP_NAME
#  define HTTPD_UPDATE_DVBS_DVBS_LNB        JOP_LNB
#  define HTTPD_UPDATE_DVBS_DVBS_LNBF_LO    JOP_LNBF_LO
#  define HTTPD_UPDATE_DVBS_DVBS_LNBF_HI    JOP_LNBF_HI
#  define HTTPD_UPDATE_DVBS_DVBS_LNBF_REF   JOP_LNBF_REF
#  define HTTPD_UPDATE_DVBS_DVBS_LNBP       JOP_LNBP
#  define HTTPD_UPDATE_DVBS_DVBS_22KHZ      JOP_22KHZ
#  define HTTPD_UPDATE_DVBS_DVBS_DISEQC     JOP_DISEQC
#  define HTTPD_UPDATE_DVBS_DVBS_DISEQC_PORT    JOP_DISEQC_PORT
#  define HTTPD_UPDATE_DVBS_DVBS_SAT_POS    JOP_SAT_POS
#  define HTTPD_UPDATE_DVBS_DVBS_KEY_TP     JOP_KEY_TP
#define STBD_UPDATE_DVBS                    JOP_UPDATE_DVBS
# define STBD_UPDATE_DVBS_MY_IP             STBD_MY_IP
# define STBD_UPDATE_DVBS_ANSWER            JOP_ANSWER
# define STBD_UPDATE_DVBS_SAT_ID            JOP_SAT_ID

/* "?opcode=delete_dvbs&sat_id=1" */
#define HTTPD_DELETE_DVBS                   JOP_DELETE_DVBS
# define HTTPD_DELETE_DVBS_MY_IP            HTTPD_MY_IP
# define HTTPD_DELETE_DVBS_SAT_ID           JOP_SAT_ID
#define STBD_DELETE_DVBS                    JOP_DELETE_DVBS
# define STBD_DELETE_DVBS_MY_IP             STBD_MY_IP
# define STBD_DELETE_DVBS_ANSWER            JOP_ANSWER
# define STBD_DELETE_DVBS_SAT_ID            JOP_SAT_ID

/* "?opcode=add_dvbs_tp&freq=10780&sym=22000&org_netid=0&netid=0&tsid=0&polar=ver&delivery=dvbs2&modulation=8psk&sat_id=1" */
#define HTTPD_ADD_DVBS_TP                   JOP_ADD_DVBS_TP
# define HTTPD_ADD_DVBS_TP_MY_IP            HTTPD_MY_IP
# define HTTPD_ADD_DVBS_TP_DVBS_TP          JOP_DVBS_TP
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_FREQ    JOP_FREQ
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_SYM     JOP_SYM
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_ORG_NETID   JOP_ORG_NETID
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_NETID       JOP_NETID
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_TSID        JOP_TSID
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_POLAR       JOP_POLAR
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_DELIVERY    JOP_DELIVERY
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_MODULATION  JOP_MODULATION
#  define HTTPD_ADD_DVBS_TP_DVBS_TP_SAT_ID      JOP_SAT_ID
#define STBD_ADD_DVBS_TP                    JOP_ADD_DVBS_TP
# define STBD_ADD_DVBS_TP_MY_IP             STBD_MY_IP
# define STBD_ADD_DVBS_TP_ANSWER            JOP_ANSWER
# define STBD_ADD_DVBS_TP_RF_ID             JOP_RF_ID

/* "?opcode=update_dvbs_tp&rf_id=1&freq=10780&sym=22000&org_netid=0&netid=0&tsid=0&polar=ver&delivery=dvbs2&modulation=8psk" */
#define HTTPD_UPDATE_DVBS_TP                        JOP_UPDATE_DVBS_TP
# define HTTPD_UPDATE_DVBS_TP_MY_IP                 HTTPD_MY_IP
# define HTTPD_UPDATE_DVBS_TP_DVBS_TP               JOP_DVBS_TP
#  define HTTPD_UPDATE_DVBS_TP_DVBS_RF_ID           JOP_RF_ID
#  define HTTPD_UPDATE_DVBS_TP_DVBS_TP_FREQ         JOP_FREQ
#  define HTTPD_UPDATE_DVBS_TP_DVBS_TP_SYM          JOP_SYM
#  define HTTPD_UPDATE_DVBS_TP_DVBS_TP_ORG_NETID    JOP_ORG_NETID
#  define HTTPD_UPDATE_DVBS_TP_DVBS_TP_NETID        JOP_NETID
#  define HTTPD_UPDATE_DVBS_TP_DVBS_TP_TSID         JOP_TSID
#  define HTTPD_UPDATE_DVBS_TP_DVBS_TP_POLAR        JOP_POLAR
#  define HTTPD_UPDATE_DVBS_TP_DVBS_TP_DELIVERY     JOP_DELIVERY
#  define HTTPD_UPDATE_DVBS_TP_DVBS_TP_MODULATION   JOP_MODULATION
#define STBD_UPDATE_DVBS_TP                 JOP_UPDATE_DVBS_TP
# define STBD_UPDATE_DVBS_TP_MY_IP          STBD_MY_IP
# define STBD_UPDATE_DVBS_TP_ANSWER         JOP_ANSWER
# define STBD_UPDATE_DVBS_TP_RF_ID          JOP_RF_ID

/* "?opcode=delete_dvbs_tp&rf_id=1" */
#define HTTPD_DELETE_DVBS_TP                JOP_DELETE_DVBS_TP
# define HTTPD_DELETE_DVBS_TP_MY_IP         HTTPD_MY_IP
# define HTTPD_DELETE_DVBS_TP_RF_ID         JOP_RF_ID
#define STBD_DELETE_DVBS_TP                 JOP_DELETE_DVBS_TP
# define STBD_DELETE_DVBS_TP_MY_IP          STBD_MY_IP
# define STBD_DELETE_DVBS_TP_ANSWER         JOP_ANSWER
# define STBD_DELETE_DVBS_TP_RF_ID          JOP_RF_ID

/* "?opcode=try_lock&fe_id=0&&rf_id=5" */
#define HTTPD_TRY_LOCK                      JOP_TRY_LOCK
# define HTTPD_TRY_LOCK_MY_IP               HTTPD_MY_IP
# define HTTPD_TRY_LOCK_FE_ID               JOP_FE_ID
# define HTTPD_TRY_LOCK_RF_ID               JOP_RF_ID
#define STBD_TRY_LOCK                       JOP_TRY_LOCK
# define STBD_TRY_LOCK_MY_IP                STBD_MY_IP
# define STBD_TRY_LOCK_ANSWER               JOP_ANSWER
# define STBD_TRY_LOCK_STRENGTH             JOP_STRENGTH
# define STBD_TRY_LOCK_SNR                  JOP_SNR
# define STBD_TRY_LOCK_BER                  JOP_BER
# define STBD_TRY_LOCK_UNC                  JOP_UNC

/* "?opcode=lock_check&fe_id=0&rf_id=1" */
#define HTTPD_LOCK_CHECK                    JOP_LOCK_CHECK
# define HTTPD_LOCK_CHECK_MY_IP             HTTPD_MY_IP
# define HTTPD_LOCK_CHECK_FE_ID             JOP_FE_ID
#define STBD_LOCK_CHECK                     JOP_LOCK_CHECK
# define STBD_LOCK_CHECK_MY_IP              STBD_MY_IP
# define STBD_LOCK_CHECK_ANSWER             JOP_ANSWER
# define STBD_LOCK_CHECK_FE_ID              JOP_FE_ID
# define STBD_LOCK_CHECK_FE_TYPE            JOP_TYPE
// for DVB-S/S2, DVB-C, DVB-T/T2, ATSC
# define STBD_LOCK_CHECK_FREQ               JOP_FREQ
// for DVB-S/S2, DVB-C
# define STBD_LOCK_CHECK_SYM                JOP_SYM
// for DVB-C, DVB-T/T2, ATSC
# define STBD_LOCK_CHECK_MODULATION         JOP_MODULATION
// for DVB-T/T2
# define STBD_LOCK_CHECK_BW                 JOP_BW
// common
# define STBD_LOCK_CHECK_STRENGTH           JOP_STRENGTH
# define STBD_LOCK_CHECK_SNR                JOP_SNR
# define STBD_LOCK_CHECK_BER                JOP_BER
# define STBD_LOCK_CHECK_UNC                JOP_UNC

/* base : "?opcode=motor_move&fe_id=0&sat_id=0&type=..."
 * step move    : "&type=step&step=E000"
 * angular move : "&type=angle&angle=E000"
 * goto N       : "&type=goto&goto=00"
 * store N      : "&type=store&store=00"
 * usals        : "&type=usals&usals=E000&longitude=E000&latitude=N000" (usals:satellite position, longitude/latitude:dish position)
 * stop         : "&type=stop"
 */
#define HTTPD_MOTOR_MOVE                    JOP_MOTOR_MOVE
# define HTTPD_MOTOR_MOVE_MY_IP             HTTPD_MY_IP
# define HTTPD_MOTOR_MOVE_FE_ID             JOP_FE_ID
# define HTTPD_MOTOR_MOVE_SAT_ID            JOP_SAT_ID
# define HTTPD_MOTOR_MOVE_TYPE              JOP_TYPE
// moving with steps
#  define HTTPD_MOTOR_MOVE_STEP             JOP_STEP
// moving with angular
#  define HTTPD_MOTOR_MOVE_ANGLE            JOP_ANGLE
// goto N
#  define HTTPD_MOTOR_MOVE_GOTO_N           JOP_GOTO
// store N
#  define HTTPD_MOTOR_MOVE_STORE_N          JOP_STORE
// usals
#  define HTTPD_MOTOR_MOVE_USALS            JOP_USALS
#  define HTTPD_MOTOR_MOVE_LONGITUDE        JOP_LONGITUDE
#  define HTTPD_MOTOR_MOVE_LATITUDE         JOP_LATITUDE
// stop
#  define HTTPD_MOTOR_MOVE_STOP             JOP_STOP
#define STBD_MOTOR_MOVE                     JOP_MOTOR_MOVE
# define STBD_MOTOR_MOVE_MY_IP              STBD_MY_IP
# define STBD_MOTOR_MOVE_ANSWER             JOP_ANSWER

/* "?opcode=scan&fe_id=0&rf_id=1" */
#define HTTPD_SCAN                          JOP_SCAN
# define HTTPD_SCAN_MY_IP                   HTTPD_MY_IP
# define HTTPD_SCAN_FE_ID                   JOP_FE_ID
# define HTTPD_SCAN_RF_ID                   JOP_RF_ID
#define STBD_SCAN                           JOP_SCAN
# define STBD_SCAN_MY_IP                    STBD_MY_IP
# define STBD_SCAN_COUNT                    JOP_COUNT
# define STBD_SCAN_CHANNEL                  JOP_CHANNEL
// array of "ix_channel_t"

/* "?opcode=channel_list" : all channels
 * "&fe_id=0"             : fe_id filtering
 * "&fe_id=0&rf_id=1"     : fe_id and rf_id filtering
 */
#define HTTPD_CHANNEL_LIST                  JOP_CHANNEL_LIST
# define HTTPD_CHANNEL_LIST_MY_IP           HTTPD_MY_IP
# define HTTPD_CHANNEL_LIST_FE_ID           JOP_FE_ID
# define HTTPD_CHANNEL_LIST_RF_ID           JOP_RF_ID
#define STBD_CHANNEL_LIST                   JOP_CHANNEL_LIST
# define STBD_CHANNEL_LIST_MY_IP            STBD_MY_IP
# define STBD_CHANNEL_LIST_FE_TYPE          JOP_TYPE
# define STBD_CHANNEL_LIST_RF_ID            JOP_RF_ID
# define STBD_CHANNEL_LIST_COUNT            JOP_COUNT
# define STBD_CHANNEL_LIST_CHANNEL          JOP_CHANNEL
// array of "ix_channel_t"

/* "?opcode=streaming&type=http&fe_id=0&ch_id=1"
 * "?opcode=streaming&type=udp&port=1234&ttl=1&fe_id=0&ch_id=1"
 * "?opcode=streaming&type=multicast&port=1234&ttl=1&path=...."
 * "?opcode=streaming&stop=xxxxx"
 */
#define HTTPD_STREAMING                     JOP_STREAMING
# define HTTPD_STREAMING_MY_IP              HTTPD_MY_IP
# define HTTPD_STREAMING_TYPE               JOP_TYPE
// start streaming
# define HTTPD_STREAMING_PORT               JOP_PORT
# define HTTPD_STREAMING_TTL                JOP_TTL
// live streaming
#  define HTTPD_STREAMING_FE_ID             JOP_FE_ID
#  define HTTPD_STREAMING_CH_ID             JOP_CH_ID
// file streaming
#  define HTTPD_STREAMING_PATH              JOP_PATH
// stop
# define HTTPD_STREAMING_STOP               JOP_STOP
#define STBD_STREAMING                      JOP_STREAMING
# define STBD_STREAMING_MY_IP               STBD_MY_IP
# define STBD_STREAMING_ANSWER              JOP_ANSWER
# define STBD_STREAMING_HANDLE              JOP_HANDLE

/* "?opcode=recording"
 * "&fe_id=0"           : TP recording
 * "&fe_id=0&ch_id=1"   : CH recording
 * "&stop=xxxx"
 */
#define HTTPD_RECORDING                     JOP_RECORDING
# define HTTPD_RECORDING_MY_IP              HTTPD_MY_IP
// start capture
# define HTTPD_RECORDING_FE_ID              JOP_FE_ID
# define HTTPD_RECORDING_CH_ID              JOP_CH_ID
// stop capture
# define HTTPD_RECORDING_STOP               JOP_STOP
#define STBD_RECORDING                      JOP_RECORDING
# define STBD_RECORDING_MY_IP               STBD_MY_IP
# define STBD_RECORDING_ANSWER              JOP_ANSWER
# define STBD_RECORDING_HANDLE              JOP_HANDLE

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IX_JSONOP_H_ */

/* end of file ****************************************************************************************************** */
