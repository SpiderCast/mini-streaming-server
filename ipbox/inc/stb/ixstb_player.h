/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSTB_PLAYER_H_
#define IXSTB_PLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"

/* ****************************************************************************************************************** */

typedef enum _ix_playtype_t {
    _playtype_playing_live,
    _playtype_playing_file,
    _playtype_recording_live,
    _playtype_streaming_live,
    _playtype_streaming_file,
    _playtype_end
} ix_playtype_t;

typedef struct _ix_playing_param_live_t {
    int     fe_id;
    int     ch_id;
} ix_player_param_live_t;

typedef struct _ix_playing_param_file_t {
    char    *path;
} ix_playing_param_file_t;

typedef struct _ix_recording_param_live_t {
    int     fe_id;
    int     ch_id;
} ix_recording_param_live_t;

typedef struct _ix_streaming_param_live_t {
    ix_streaming_t  type;
    char    ip[16];
    int     port;
    int     ttl;
    int     fe_id;
    int     ch_id;
} ix_streaming_param_live_t;

typedef struct _ix_streaming_param_file_t {
    ix_streaming_t  type;
    char    ip[16];
    int     port;
    int     ttl;
    char    *path;
} ix_streaming_param_file_t;

typedef union _ix_player_param_t {
    ix_player_param_live_t      play_live;
    ix_playing_param_file_t     play_file;
    ix_recording_param_live_t   record_live;
    ix_streaming_param_live_t   stream_live;
    ix_streaming_param_file_t   stream_file;
} ix_player_param_t;

typedef struct _ix_player_handle_t {
    void        *local;
} *ix_player_handle_t;

/* ****************************************************************************************************************** */

extern ix_player_handle_t   ixstb_player_request(ix_playtype_t type, ix_player_param_t *param);
extern int  ixstb_player_retract(ix_player_handle_t handle);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSTB_PLAYER_H_ */

/* end of file ****************************************************************************************************** */
