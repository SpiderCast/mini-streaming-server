/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_scan.h"
#include "ixsys_print.h"
#include "ixsys_msgq.h"
#include "ixstb_scan_mpeg.h"
#include "ixstb_scan_dvb.h"
#include "ixstb_fe.h"
#include "ixsys_db_dvbs.h"
#include "ixsys_db_channel.h"
#include "ixsys_iconv.h"
#include <libucsi/mpeg/types.h>
#include <libucsi/mpeg/section.h>
#include <libucsi/dvb/section.h>
#include <libucsi/mpeg/descriptor.h>
#include <libucsi/dvb/descriptor.h>

/* ****************************************************************************************************************** */

#define MULTI_SECTION_LIMIT     20
#define SECTION_SIZE            4096
#define SCAN_MSG_LIMIT          20

/* ****************************************************************************************************************** */

#define SCAN_CB_MACRO(cb, a, b, c, d)   \
            if (cb) (cb)(a, b, c, d)

/* ****************************************************************************************************************** */

typedef struct _ixstb_scan_msg_t {
    int         section_no;
    size_t      section_len;
} ixstb_scan_msg_t;

typedef struct _ixstb_scan_priv_t {
    ix_msgq_handle_t    msgq;
    uint16_t    pid;
    int         section_no;
} ixstb_scan_priv_t;

typedef struct _ixstb_scan_local_t {
    int     fe_id;
    int     rf_id;

    pthread_t           thread;

    ix_scan_param_t     param;
    ix_msgq_handle_t    msgq;

    ix_section_handle_t section_pat;
    uint8_t             pat_buffer[SECTION_SIZE];
    size_t              pat_size;
    ixstb_scan_priv_t   pat_priv;
    ix_section_handle_t section_sdt;
    uint8_t             sdt_buffer[SECTION_SIZE];
    size_t              sdt_size;
    ixstb_scan_priv_t   sdt_priv;
    ix_section_handle_t section_pmt[MULTI_SECTION_LIMIT];
    uint8_t             pmt_buffer[MULTI_SECTION_LIMIT][SECTION_SIZE];
    size_t              pmt_size[MULTI_SECTION_LIMIT];
    ixstb_scan_priv_t   pmt_priv[MULTI_SECTION_LIMIT];
} ixstb_scan_local_t;

/* ****************************************************************************************************************** */

static void section_callback(size_t section_len, void *priv)
{
    ixstb_scan_priv_t   *scan_priv = (ixstb_scan_priv_t*)priv;

    if (scan_priv) {
        ixstb_scan_msg_t    msg;

        memset(&msg, 0, sizeof(msg));

        msg.section_no  = scan_priv->section_no;
        msg.section_len = section_len;
        ixsys_msgq_send(scan_priv->msgq, &msg);
    }
}

static bool find_channel(struct mpeg_pmt_section *pmt, struct dvb_sdt_section *sdt, ix_channel_t *new_ch)
{
    struct mpeg_pmt_stream  *cur_stream;
    struct dvb_sdt_service  *cur_service;
    struct descriptor       *cur_desc;
    bool    found = false;

    sprintf(new_ch->name, "%d[%x]", mpeg_pmt_section_program_number(pmt), mpeg_pmt_section_program_number(pmt));
    new_ch->pcr_pid = pmt->pcr_pid;
    new_ch->service_id = mpeg_pmt_section_program_number(pmt);

    mpeg_pmt_section_descriptors_for_each(pmt, cur_desc) {
        if (cur_desc->tag == dtag_mpeg_ca) {
            struct mpeg_ca_descriptor   *ca_desc = mpeg_ca_descriptor_codec(cur_desc);
            new_ch->cas_id = ca_desc->ca_system_id;
        }
    }

    mpeg_pmt_section_streams_for_each(pmt, cur_stream) {
        switch (cur_stream->stream_type) {
        case MPEG_STREAM_TYPE_ISO11172_VIDEO:
        case MPEG_STREAM_TYPE_ISO13818_2_VIDEO:
        case MPEG_STREAM_TYPE_ISO14496_2_VISUAL:        // MPEG-4 part 2 video
        case MPEG_STREAM_TYPE_ISO14496_10_VIDEO:        // H.264 (MPEG-4/AVC)
            new_ch->vid_pid  = cur_stream->pid;
            new_ch->vid_type = cur_stream->stream_type;
            break;
        case MPEG_STREAM_TYPE_ISO11172_AUDIO:
        case MPEG_STREAM_TYPE_ISO13818_3_AUDIO:
        case MPEG_STREAM_TYPE_ISO13818_7_AUDIO_ADTS:    // AAC
        case MPEG_STREAM_TYPE_ISO14496_3_AUDIO_LATM:    // HE-AAC
            new_ch->aud_pid  = cur_stream->pid;
            new_ch->aud_type = cur_stream->stream_type;
            break;
        }

        mpeg_pmt_stream_descriptors_for_each(cur_stream, cur_desc) {
            switch (cur_desc->tag) {
            case dtag_mpeg_ca:
                new_ch->cas_id = mpeg_ca_descriptor_codec(cur_desc)->ca_system_id;
                break;
            case dtag_dvb_ac3:
            case dtag_dvb_enhanced_ac3_descriptor:
                new_ch->aud_pid  = cur_stream->pid;
                new_ch->aud_type = cur_desc->tag;
                break;
            }
        }
    }

    dvb_sdt_section_services_for_each(sdt, cur_service) {
        if (cur_service->service_id == mpeg_pmt_section_program_number(pmt)) {
            // print_debug("found SDT section\n");
            // print_debug("\tservice_id                : 0x%x\n", cur_service->service_id);
            // print_debug("\teit_schedule_flag         : %d\n", cur_service->eit_schedule_flag);
            // print_debug("\teit_present_following_flag: %d\n", cur_service->eit_present_following_flag);
            // print_debug("\trunning_status            : %d\n", cur_service->running_status);
            // print_debug("\tfree_ca_mode              : %d\n", cur_service->free_ca_mode);

            dvb_sdt_service_descriptors_for_each(cur_service, cur_desc) {
                if (cur_desc->tag == dtag_dvb_service) {
                    struct dvb_service_descriptor       *dx;
                    struct dvb_service_descriptor_part2 *part2;

                    if (((dx = dvb_service_descriptor_codec(cur_desc)) != NULL) && ((part2 = dvb_service_descriptor_part2(dx)) != NULL)) {
                        char    name[CHANNEL_NAME_STRLEN];
                        size_t  len = sizeof(name);

                        if (len > part2->service_name_length)
                            len = part2->service_name_length + 1;

                        snprintf(name, len, "%s", dvb_service_descriptor_service_name(part2));
                        ixsys_iconv_dvb_to_utf8(name, new_ch->name, sizeof(new_ch->name));

                        found = true;

                        break;
                    }
                }
            }
        }

        if (found == true)
            break;
    }

    return found;
}

static void* get_scan(void *param)
{
    ixstb_scan_local_t  *local = (ixstb_scan_local_t*)param;
    ixstb_scan_msg_t    msg;
    ix_dvbs_tp_t        update_tp;
    ix_channel_t        new_ch;
    struct section      *section;
    struct section_ext  *section_ext = NULL;
    struct mpeg_pat_section *pat = NULL;
    struct mpeg_pat_program *cur_pat = NULL;
    struct dvb_sdt_section  *sdt = NULL;
    struct mpeg_pmt_section *pmt = NULL;
    int     n, o;
    int     count = 0;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    /* PAT */

    local->pat_priv.msgq = local->msgq;
    local->pat_priv.pid  = TRANSPORT_PAT_PID;
    local->pat_priv.section_no = 0;

    local->section_pat = ixstb_scan_section_pat(local->fe_id, local->pat_buffer, SECTION_SIZE, section_callback, (void*)&local->pat_priv);
    memset(&msg, 0, sizeof(msg));
    ixsys_msgq_recv(local->msgq, &msg, 0/*PAT_TIMEOUT + 1000*/);
    ixstb_section_stop(local->section_pat);
    local->section_pat = NULL;

    if ((msg.section_len == 0) || ((section = section_codec(local->pat_buffer, msg.section_len)) == NULL) || ((section_ext = section_ext_decode(section, 1)) == NULL)) {
        print_error("PAT receiving error");
        SCAN_CB_MACRO(local->param.cb, _scan_step_error, (void*)"PAT", strlen("PAT"), local->param.priv);
        return NULL;
    }

    local->pat_size = msg.section_len;

    pat = mpeg_pat_section_codec(section_ext);
    print_debug("PAT: transport_stream_id: 0x%x\n", mpeg_pat_section_transport_stream_id(pat));

    SCAN_CB_MACRO(local->param.cb, _scan_step_receive_pat, (void*)"PAT", strlen("PAT"), local->param.priv);

    /* SDT */

    local->sdt_priv.msgq = local->msgq;
    local->pat_priv.pid  = TRANSPORT_SDT_PID;
    local->sdt_priv.section_no = 0;

    local->section_sdt = ixstb_scan_section_sdt(local->fe_id, mpeg_pat_section_transport_stream_id(pat), local->sdt_buffer, SECTION_SIZE, section_callback, (void*)&local->sdt_priv);
    memset(&msg, 0, sizeof(msg));
    ixsys_msgq_recv(local->msgq, &msg, 0/*SDT_TIMEOUT + 1000*/);
    ixstb_section_stop(local->section_sdt);
    local->section_sdt = NULL;

    if ((msg.section_len == 0) || ((section = section_codec(local->sdt_buffer, msg.section_len)) == NULL) || ((section_ext = section_ext_decode(section, 1)) == NULL)) {
        print_warn("SDT receive error");
        SCAN_CB_MACRO(local->param.cb, _scan_step_timeout, (void*)"SDT", strlen("SDT"), local->param.priv);
    }

    if ((local->sdt_size = msg.section_len) > 0) {
        sdt = dvb_sdt_section_codec(section_ext);
        print_debug("SDT: transport_stream_id: 0x%x / original_network_id: 0x%x\n", dvb_sdt_section_transport_stream_id(sdt), sdt->original_network_id);
    }

    SCAN_CB_MACRO(local->param.cb, _scan_step_receive_sdt, (void*)"SDT", strlen("SDT"), local->param.priv);

    /* update TP */

    ixsys_get_dvbs_tp(DB_PATH_DATA, local->rf_id, &update_tp);
    update_tp.tsid = mpeg_pat_section_transport_stream_id(pat);
    if (sdt)
        update_tp.org_netid = sdt->original_network_id;
    ixsys_update_dvbs_tp(DB_PATH_DATA, &update_tp);

    /* PMT */

    n = 0;

    mpeg_pat_section_programs_for_each(pat, cur_pat) {
        if (cur_pat->pid != 0) {
            local->pmt_priv[n].msgq = local->msgq;
            local->pmt_priv[n].pid  = cur_pat->pid;
            local->pmt_priv[n].section_no = n;
            local->section_pmt[n] = ixstb_scan_section_pmt(local->fe_id, cur_pat->pid, cur_pat->program_number, local->pmt_buffer[n], SECTION_SIZE, section_callback, (void*)&local->pmt_priv[n]);
            n++;
        }

        if (n == MULTI_SECTION_LIMIT) {
            for ( ; n > 0; n--) {
                memset(&msg, 0, sizeof(msg));
                ixsys_msgq_recv(local->msgq, &msg, 0/*PMT_TIMEOUT + 1000*/);

                for (o = 0; o < MULTI_SECTION_LIMIT; o++) {
                    if (local->pmt_priv[o].section_no == msg.section_no) {
                        ixstb_section_stop(local->section_pmt[msg.section_no]);
                        local->section_pmt[msg.section_no] = NULL;
                        local->pmt_size[msg.section_no] = msg.section_len;

                        /* receive PMT */
                        if ((msg.section_len == 0) || ((section = section_codec(local->pmt_buffer[msg.section_no], msg.section_len)) == NULL) || ((section_ext = section_ext_decode(section, 1)) == NULL)) {
                            print_warn("PMT receive error: 0x%x", local->pmt_priv[msg.section_no].pid);
                            SCAN_CB_MACRO(local->param.cb, _scan_step_timeout, (void*)"PMT", strlen("PMT"), local->param.priv);
                        } else {
                            pmt = mpeg_pmt_section_codec(section_ext);
                            print_debug("PMT: program_number: 0x%x / pcr_pid: 0x%x\n", mpeg_pmt_section_program_number(pmt), pmt->pcr_pid);

                            SCAN_CB_MACRO(local->param.cb, _scan_step_receive_pmt, (void*)"PMT", strlen("PMT"), local->param.priv);

                            memset(&new_ch, 0, sizeof(new_ch));

                            // print_dump("PMT", local->pmt_buffer[msg.section_no], local->pmt_size[msg.section_no]);
                            if (find_channel(pmt, sdt, &new_ch) == true)
                                count++;

                            new_ch.rf_id = local->rf_id;
                            ixstb_fe_get_type(local->fe_id, &new_ch.fe_type);

                            ixsys_add_channel(local->param.db_path, &new_ch);
                        }

                        break;
                    }
                }
            }
        }
    }

    for ( ; n > 0; n--) {
        memset(&msg, 0, sizeof(msg));
        ixsys_msgq_recv(local->msgq, &msg, 0/*PMT_TIMEOUT + 1000*/);

        for (o = 0; o < MULTI_SECTION_LIMIT; o++) {
            if (local->pmt_priv[o].section_no == msg.section_no) {
                ixstb_section_stop(local->section_pmt[msg.section_no]);
                local->section_pmt[msg.section_no] = NULL;
                local->pmt_size[msg.section_no] = msg.section_len;

                /* receive PMT */
                if ((msg.section_len == 0) || ((section = section_codec(local->pmt_buffer[msg.section_no], msg.section_len)) == NULL) || ((section_ext = section_ext_decode(section, 1)) == NULL)) {
                    print_warn("PMT receive error: 0x%x", local->pmt_priv[msg.section_no].pid);
                    SCAN_CB_MACRO(local->param.cb, _scan_step_timeout, (void*)"PMT", strlen("PMT"), local->param.priv);
                } else {
                    pmt = mpeg_pmt_section_codec(section_ext);
                    print_debug("PMT: program_number: 0x%x / pcr_pid: 0x%x\n", mpeg_pmt_section_program_number(pmt), pmt->pcr_pid);

                    SCAN_CB_MACRO(local->param.cb, _scan_step_receive_pmt, (void*)"PMT", strlen("PMT"), local->param.priv);

                    memset(&new_ch, 0, sizeof(new_ch));

                    // print_dump("PMT", local->pmt_buffer[msg.section_no], local->pmt_size[msg.section_no]);
                    if (find_channel(pmt, sdt, &new_ch) == true)
                        count++;

                    new_ch.rf_id = local->rf_id;
                    ixstb_fe_get_type(local->fe_id, &new_ch.fe_type);

                    ixsys_add_channel(local->param.db_path, &new_ch);
                }

                break;
            }
        }
    }

    SCAN_CB_MACRO(local->param.cb, _scan_step_done, (void*)&count, sizeof(count), local->param.priv);

    return NULL;
}

ix_scan_handle_t ixstb_scan_request(int fe_id, int rf_id, ix_scan_param_t *param)
{
    ix_scan_handle_t    handle;
    ixstb_scan_local_t  *local;
    pthread_t   thr;
    int         n;

    local = (ixstb_scan_local_t*)calloc(1, sizeof(ixstb_scan_local_t));

    local->fe_id = fe_id;
    local->rf_id = rf_id;
    local->param = *param;
    local->msgq  = ixsys_msgq_create(sizeof(ixstb_scan_msg_t), SCAN_MSG_LIMIT);

    if ((n = pthread_create(&thr, NULL, get_scan, (void*)local)) != 0) {
        print_error("gather scan create failed [%x]", n);

        ixsys_msgq_destroy(local->msgq);
        free(local);

        return NULL;
    }
    local->thread = thr;

    handle = (ix_scan_handle_t)calloc(1, sizeof(struct _ix_scan_handle_t));
    handle->local = local;

    return handle;
}

int ixstb_scan_retract(ix_scan_handle_t handle)
{
    if (handle) {
        void    *res;
        int     n;
        ixstb_scan_local_t  *local = handle->local;

        if (local->section_pat)
            ixstb_section_stop(local->section_pat);
        if (local->section_sdt)
            ixstb_section_stop(local->section_sdt);

        for (n = 0; n < MULTI_SECTION_LIMIT; n++) {
            if (local->section_pmt[n])
                ixstb_section_stop(local->section_pmt[n]);
        }

        pthread_cancel(local->thread);
        pthread_join(local->thread, &res);

        ixsys_msgq_destroy(local->msgq);

        free(local);
        free(handle);
    }

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
