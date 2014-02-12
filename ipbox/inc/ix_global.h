/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IX_GLOBAL_H_
#define IX_GLOBAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#define DB_VERSION              2014012101

#define CGI_TO_STB_PATH         "/tmp/path_cgi_to_stb"
#define STB_TO_CGI_PATH         "/tmp/path_stb_to_cgi"

#define DB_PATH_DATA_v1         "/var/data/data_v1.db"
#define DB_PATH_DATA_v2         "/var/data/data_v2.db"
#define DB_PATH_DATA            DB_PATH_DATA_v2
#define DB_PATH_DEFAULT_DATA_v1 "/var/data/default_v1.db"
#define DB_PATH_DEFAULT_DATA_v2 "/var/data/default_v2.db"
#define DB_PATH_DEFAULT_DATA    DB_PATH_DEFAULT_DATA_v2
#define DB_PATH_COPY            "/var/www/data.db"

#define DEFAULT_REC_PATH        "/var/data"
#define DEFAULT_WEB_PATH        "/var/www"

/* ****************************************************************************************************************** */

typedef enum _ix_streaming_t {
    _streaming_http,
    _streaming_udp,
    _streaming_multicast,
    _streaming_end
} ix_streaming_t;

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IX_GLOBAL_H_ */

/* end of file ****************************************************************************************************** */
