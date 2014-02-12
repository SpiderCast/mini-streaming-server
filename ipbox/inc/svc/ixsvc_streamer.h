/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSVC_STREAMER_H_
#define IXSVC_STREAMER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"

/* ****************************************************************************************************************** */

typedef struct _ix_streamer_handle_t {
    void        *local;
} *ix_streamer_handle_t, *ix_recorder_handle_t;

/* ****************************************************************************************************************** */

extern ix_streamer_handle_t ixsvc_streamer_request(ix_streaming_t type, char *ip, uint16_t port, int ttl, int fd, char *uri);
extern int  ixsvc_streamer_retract(ix_streamer_handle_t handle, char *uri);
extern int  ixsvc_streamer_get_duration_info(ix_streamer_handle_t handle, ssize_t *sent_bytes, int *duration_sec);
extern ix_recorder_handle_t ixsvc_recorder_request(char *path, int fd);
extern int  ixsvc_recorder_retract(ix_recorder_handle_t handle);
extern int  ixsvc_recorder_get_duration_info(ix_recorder_handle_t handle, ssize_t *written_bytes, int *duration_sec);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSVC_STREAMER_H_ */

/* end of file ****************************************************************************************************** */
