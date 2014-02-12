/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_player.h"
#include "ixsys_print.h"
#include "ixsys_db.h"
#include "ixsys_msgq.h"
#include "ixsys_db_channel.h"
#include "ixstb_fe.h"
#include "ixstb_dmx.h"
#include "ixstb_scan_mpeg.h"
#include "ixstb_scan_dvb.h"
#include "ixsvc_streamer.h"
#include "ixstb_fakepat.h"
#include "ixsvc_epgmgr.h"
#include <libucsi/mpeg/section.h>
#include <libucsi/dvb/section.h>
#include <libucsi/mpeg/types.h>
#include <libucsi/mpeg/descriptor.h>
#include <libucsi/dvb/descriptor.h>
#include <time.h>

/* ****************************************************************************************************************** */

#define SECTION_SIZE        4096
#define SCAN_MSG_LIMIT      20

#define DVR_BUFFER_SIZE     (1024 * 1024)

typedef struct _ixstb_streaming_msg_t {
    size_t      section_len;
} ixstb_streaming_msg_t;

typedef struct _ixstb_streaming_priv_t {
    ix_msgq_handle_t    msgq;
} ixstb_streaming_priv_t;

typedef struct _ixstb_streaming_local_t {
    ix_playtype_t       type;

    int                 fe_id;

    /* get section info. */

    ix_section_handle_t section_pat;
    uint8_t             pat_buffer[SECTION_SIZE];
    size_t              pat_size;
    ix_section_handle_t section_pmt;
    uint8_t             pmt_buffer[SECTION_SIZE];
    size_t              pmt_size;
    ixstb_streaming_priv_t   priv;

    ix_fakepat_handle_t fake_pat;

    /* for streaming/recording */

#define STREAMING_DMX_LIMIT     16
    int     dvr_fd;
    int     dmx_fd[STREAMING_DMX_LIMIT];
    int     dmx_cnt;
    ix_streamer_handle_t    streamer;
} ixstb_streaming_local_t;

/* ****************************************************************************************************************** */

static void section_callback(size_t section_len, void *priv)
{
    ixstb_streaming_priv_t  *streaming_priv = (ixstb_streaming_priv_t*)priv;

    if (streaming_priv) {
        ixstb_streaming_msg_t   msg;

        memset(&msg, 0, sizeof(msg));

        msg.section_len = section_len;
        ixsys_msgq_send(streaming_priv->msgq, &msg);
    }
}

static ix_player_handle_t create_streaming_live_dvb(int fe_id, ix_channel_t *ch)
{
    ix_player_handle_t      handle = NULL;
    ixstb_streaming_local_t *local;
    ixstb_streaming_msg_t   msg;
    struct section          *section;
    struct section_ext      *section_ext = NULL;
    struct mpeg_pat_section *pat = NULL;
    struct mpeg_pat_program *cur_pat = NULL;
    struct mpeg_pmt_section *pmt = NULL;
    struct mpeg_pmt_stream  *cur_stream;
    struct descriptor       *cur_desc;
    uint16_t    pmt_pid = 0;
    int         n = 0;

    local = (ixstb_streaming_local_t*)calloc(1, sizeof(ixstb_streaming_local_t));

    /* streaming/recording ready */

    local->dvr_fd = ixstb_dmx_open_dvr(fe_id, DVR_BUFFER_SIZE);

    /* epg gathering ready */

    local->fe_id  = fe_id;

    if (ch) {
        local->priv.msgq = ixsys_msgq_create(sizeof(ixstb_streaming_msg_t), SCAN_MSG_LIMIT);

        /* get PMT pid */

        local->section_pat = ixstb_scan_section_pat(fe_id, local->pat_buffer, SECTION_SIZE, section_callback, (void*)&local->priv);
        memset(&msg, 0, sizeof(msg));
        ixsys_msgq_recv(local->priv.msgq, &msg, 0/*PAT_TIMEOUT + 1000*/);
        ixstb_section_stop(local->section_pat);
        local->section_pat = NULL;

        if ((msg.section_len == 0) || ((section = section_codec(local->pat_buffer, msg.section_len)) == NULL) || ((section_ext = section_ext_decode(section, 1)) == NULL)) {
            print_error("PAT receiving error");

            ixsys_msgq_destroy(local->priv.msgq);
            ixstb_dmx_close_dvr(local->dvr_fd);
            free(local);

            return NULL;
        }

        local->pat_size = msg.section_len;

        pat = mpeg_pat_section_codec(section_ext);
        print_debug("PAT: transport_stream_id: 0x%x\n", mpeg_pat_section_transport_stream_id(pat));

        mpeg_pat_section_programs_for_each(pat, cur_pat) {
            if ((cur_pat->pid != 0) && (cur_pat->program_number == ch->service_id)) {
                pmt_pid = cur_pat->pid;
                break;
            }
        }

        if (pmt_pid == 0) {
            print_error("can't found PMT :0x%x", ch->service_id);

            ixsys_msgq_destroy(local->priv.msgq);
            ixstb_dmx_close_dvr(local->dvr_fd);
            free(local);

            return NULL;
        }

        local->section_pmt = ixstb_scan_section_pmt(fe_id, pmt_pid, cur_pat->program_number, local->pmt_buffer, SECTION_SIZE, section_callback, (void*)&local->priv);
        memset(&msg, 0, sizeof(msg));
        ixsys_msgq_recv(local->priv.msgq, &msg, 0/*PMT_TIMEOUT + 1000*/);
        ixstb_section_stop(local->section_pmt);

        if ((msg.section_len == 0) || ((section = section_codec(local->pmt_buffer, msg.section_len)) == NULL) || ((section_ext = section_ext_decode(section, 1)) == NULL)) {
            print_error("PMT receiving error");

            ixsys_msgq_destroy(local->priv.msgq);
            ixstb_dmx_close_dvr(local->dvr_fd);
            free(local);

            return NULL;
        }

        // local->dmx_fd[n++] = ixstb_dmx_set_pid_filter(fe_id, TRANSPORT_SDT_PID, 0, DMX_IN_FRONTEND, DMX_OUT_TS_TAP, DMX_PES_OTHER);
        // local->dmx_fd[n++] = ixstb_dmx_set_pid_filter(fe_id, TRANSPORT_EIT_PID, 0, DMX_IN_FRONTEND, DMX_OUT_TS_TAP, DMX_PES_OTHER);
        local->dmx_fd[n++] = ixstb_dmx_set_pid_filter(fe_id, pmt_pid, 0, DMX_IN_FRONTEND, DMX_OUT_TS_TAP, DMX_PES_OTHER);

        local->fake_pat = ixstb_fakepat_create(mpeg_pat_section_transport_stream_id(pat), local->dvr_fd);
        ixstb_fakepat_add_pmt(local->fake_pat, cur_pat->program_number, pmt_pid);

        pmt = mpeg_pmt_section_codec(section_ext);

        mpeg_pmt_section_streams_for_each(pmt, cur_stream) {
            switch (cur_stream->stream_type) {
            /* video */
            case MPEG_STREAM_TYPE_ISO11172_VIDEO:
            case MPEG_STREAM_TYPE_ISO13818_2_VIDEO:
            case MPEG_STREAM_TYPE_ISO14496_2_VISUAL:        // MPEG-4 part 2 video
            case MPEG_STREAM_TYPE_ISO14496_10_VIDEO:        // H.264 (MPEG-4/AVC)
            /* audio */
            case MPEG_STREAM_TYPE_ISO11172_AUDIO:
            case MPEG_STREAM_TYPE_ISO13818_3_AUDIO:
            case MPEG_STREAM_TYPE_ISO13818_7_AUDIO_ADTS:    // AAC
            case MPEG_STREAM_TYPE_ISO14496_3_AUDIO_LATM:    // HE-AAC
                local->dmx_fd[n++] = ixstb_dmx_set_pid_filter(fe_id, cur_stream->pid, 0, DMX_IN_FRONTEND, DMX_OUT_TS_TAP, DMX_PES_OTHER);
                break;
            case MPEG_STREAM_TYPE_ISO13818_1_PRIVATE_SECTIONS:  // teletext-subtitle
            case MPEG_STREAM_TYPE_ISO13818_1_PRIVATE_PES:       // subtitle, dvb-ac3
                mpeg_pmt_stream_descriptors_for_each(cur_stream, cur_desc) {
                    switch (cur_desc->tag) {
                    case dtag_dvb_teletext:
                        {
                            struct dvb_teletext_descriptor  *ttxt_desc;
                            struct dvb_teletext_entry       *cur_entry;

                            if ((ttxt_desc = dvb_teletext_descriptor_codec(cur_desc)) != NULL) {
                                dvb_teletext_descriptor_entries_for_each(ttxt_desc, cur_entry) {
                                    if (cur_entry->type == DVB_TELETEXT_TYPE_SUBTITLE)
                                        local->dmx_fd[n++] = ixstb_dmx_set_pid_filter(fe_id, cur_stream->pid, 0, DMX_IN_FRONTEND, DMX_OUT_TS_TAP, DMX_PES_OTHER);
                                }
                            }
                        }
                        break;
                    case dtag_dvb_subtitling:
                    case dtag_dvb_ac3:
                    case dtag_dvb_enhanced_ac3_descriptor:
                        local->dmx_fd[n++] = ixstb_dmx_set_pid_filter(fe_id, cur_stream->pid, 0, DMX_IN_FRONTEND, DMX_OUT_TS_TAP, DMX_PES_OTHER);
                        break;
                    }
                }
                break;
            }

            if (n == STREAMING_DMX_LIMIT)
                break;
        }

        /* epg start */

        // ixsvc_epgmgr_start(fe_id);
    } else {
        local->dmx_fd[n++] = ixstb_dmx_set_pid_filter(fe_id, 0x2000, 0, DMX_IN_FRONTEND, DMX_OUT_TS_TAP, DMX_PES_OTHER);
    }

    local->dmx_cnt = n;
    for (n = 0; n < local->dmx_cnt; n++)
        ixstb_dmx_start_filter(local->dmx_fd[n]);

    handle = (ix_player_handle_t)calloc(1, sizeof(struct _ix_player_handle_t));
    handle->local = local;

    return handle;
}

static ix_player_handle_t create_streaming_live(ix_streaming_param_live_t *param)
{
    ix_player_handle_t  handle = NULL;
    ix_channel_t        ch;
    fe_type_t           fe_type;
    fe_status_t         status;

    if (ixstb_fe_get_type(param->fe_id, &fe_type) != 0) {
        print_error("unknown frontend id :%d", param->fe_id);
        return NULL;
    }

    if ((ixsys_get_channel(DB_PATH_DATA, param->ch_id, &ch) != 0) || (fe_type != ch.fe_type)) {
        print_error("illegal fe_id or ch_id :%d/%d", param->fe_id, param->ch_id);
        return NULL;
    }

    if (ixstb_fe_try_lock(param->fe_id, ch.rf_id) == 0) {
        sleep(2);
    }

    status = ixstb_fe_get_signal_status(param->fe_id, NULL, NULL, NULL, NULL);
    if (!(status & FE_HAS_LOCK)) {
        print_error("lock fault :%d/%d", param->fe_id, ch.rf_id);
        return NULL;
    }

    switch (fe_type) {
    case FE_QPSK:
    case FE_QAM:
    case FE_OFDM:
        handle = create_streaming_live_dvb(param->fe_id, &ch);
        break;
    case FE_ATSC:
        break;
    }

    if (handle) {
        ixstb_streaming_local_t *local = (ixstb_streaming_local_t*)handle->local;
        char    uri[128];

        snprintf(uri, sizeof(uri), DEFAULT_WEB_PATH"/streaming/%p.ts", handle);

        local->streamer = ixsvc_streamer_request(param->type, param->ip, param->port, param->ttl, local->dvr_fd, uri);
    }

    return handle;
}

static ix_player_handle_t create_recording_live(ix_recording_param_live_t *param)
{
    ix_player_handle_t  handle = NULL;
    ix_channel_t        ch, *p_ch = NULL;
    fe_type_t           fe_type;

    if (ixstb_fe_get_type(param->fe_id, &fe_type) != 0) {
        print_error("unknown frontend id :%d", param->fe_id);
        return NULL;
    }

    if (param->ch_id > 0) {
        if ((ixsys_get_channel(DB_PATH_DATA, param->ch_id, &ch) != 0) || (fe_type != ch.fe_type)) {
            print_error("illegal fe_id or ch_id :%d/%d", param->fe_id, param->ch_id);
            return NULL;
        }

        p_ch = &ch;
    }

    switch (fe_type) {
    case FE_QPSK:
    case FE_QAM:
    case FE_OFDM:
        handle = create_streaming_live_dvb(param->fe_id, p_ch);
        break;
    case FE_ATSC:
        break;
    }

    if (handle) {
        ixstb_streaming_local_t *local = (ixstb_streaming_local_t*)handle->local;
        char        path[128];
        time_t      t;
        struct tm   *ltime;

        time(&t);
        ltime = localtime(&t);

        if (p_ch) {
            snprintf(path, sizeof(path), DEFAULT_REC_PATH"/%d_%s_%s.ts", ch.service_id, ch.name, asctime(ltime));
        } else {

        }

        local->streamer = ixsvc_recorder_request(path, local->dvr_fd);
    }

    return handle;
}

static int delete_streaming_live(ixstb_streaming_local_t *local, char *uri)
{
    int     n;

    // ixsvc_epgmgr_stop(local->fe_id);

    ixsvc_streamer_retract(local->streamer, uri);
    ixstb_fakepat_delete(local->fake_pat);

    for (n = 0; n < local->dmx_cnt; n++)
        ixstb_dmx_clr_filter(local->dmx_fd[n]);
    ixstb_dmx_close_dvr(local->dvr_fd);

    return 0;
}

ix_player_handle_t ixstb_player_request(ix_playtype_t type, ix_player_param_t *param)
{
    ix_player_handle_t  handle = NULL;

    switch (type) {
    case _playtype_playing_live:
        break;
    case _playtype_playing_file:
        break;
    case _playtype_recording_live:
        handle = create_recording_live(&param->record_live);
        break;
    case _playtype_streaming_live:
        handle = create_streaming_live(&param->stream_live);
        break;
    case _playtype_streaming_file:
        break;
    default:
        return NULL;
    }

    if (handle) {
        ixstb_streaming_local_t *local = (ixstb_streaming_local_t*)handle->local;
        local->type = type;
    }

    print_debug("handle: %p\n", handle);

    return handle;
}

int ixstb_player_retract(ix_player_handle_t handle)
{
    print_debug("handle: %p\n", handle);

    if (handle) {
        ixstb_streaming_local_t *local = handle->local;
        char    uri[128];

        snprintf(uri, sizeof(uri), DEFAULT_WEB_PATH"/streaming/%p.ts", handle);

        delete_streaming_live(local, uri);

        free(local);
        free(handle);
    }

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
