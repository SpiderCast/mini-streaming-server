/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsvc_epgmgr.h"
#include "ixsys_print.h"
#include "ixsys_db.h"
#include "ixsys_msgq.h"
#include "ixstb_section.h"
#include <libucsi/dvb/section.h>
#include "ix_list.h"

/* ****************************************************************************************************************** */

#define EIT_TIMEOUT         20000
#define EIT_SECTION_SIZE    8192

typedef struct _ixsvc_epg_local_t {
    struct list_head    list;

    pthread_t           thread;
    pthread_mutex_t     locker;
    pthread_mutex_t     waiter;

    ix_section_handle_t handle;
    ix_msgq_handle_t    msgq;

    int         fe_id;

    uint8_t     tag;
    uint8_t     buffer[EIT_SECTION_SIZE];
    size_t      read_size;
} ixsvc_epg_local_t;

/* ****************************************************************************************************************** */

static ixsvc_epg_local_t    guide_list_;

/* ****************************************************************************************************************** */

static ixsvc_epg_local_t* find_node_use_fe_id(int fe_id)
{
    ixsvc_epg_local_t   *pos;

    list_for_each_entry(pos, &guide_list_.list, list) {
        if (fe_id == pos->fe_id) {
            return pos;
        }
    }

    return NULL;
}

static int load_from_network()
{
    //ixsys_get_epg_url()
    return 0;
}

static int load_from_fs()
{
    //ixsys_get_epg_filepath()
    return 0;
}

static void eit_callback(size_t read_size, void *priv)
{
    ixsvc_epg_local_t   *local = (ixsvc_epg_local_t*)priv;

    local->read_size = read_size;

    pthread_mutex_unlock(&local->locker);
    pthread_mutex_lock(&local->waiter);

    local->read_size = 0;
}

static void* get_guide(void *param)
{
    ixsvc_epg_local_t   *local = (ixsvc_epg_local_t*)param;
    uint8_t     eit[EIT_SECTION_SIZE];
    size_t      eit_size;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    while (1) {
        pthread_mutex_lock(&local->locker);

        eit_size = local->read_size;
        memcpy(eit, local->buffer, eit_size);

        pthread_mutex_unlock(&local->waiter);

        print_dump("EIT", eit, eit_size);
    }

    return NULL;
}

int ixsvc_epgmgr_init()
{
    INIT_LIST_HEAD(&guide_list_.list);

    load_from_network();
    load_from_fs();

    return 0;
}

int ixsvc_epgmgr_start(int fe_id)
{
    int         id;
    pthread_t   thr;
    uint8_t     f_bytes[1], f_masks[1];
    ixsvc_epg_local_t   *node;
    ix_section_param_t  param;

    if ((node = find_node_use_fe_id(fe_id)) != NULL) {
        ixsvc_epgmgr_stop(fe_id);
    }

    /* only schedule epg */

    node = (ixsvc_epg_local_t*)malloc(sizeof(ixsvc_epg_local_t));
    memset(node, 0, sizeof(ixsvc_epg_local_t));

    f_bytes[0] = stag_dvb_event_information_schedule_actual;
    f_masks[0] = 0xff;

    param.pid      = TRANSPORT_EIT_PID;
    param.f_bytes  = f_bytes;
    param.f_masks  = f_masks;
    param.f_size   = sizeof(f_bytes);
    param.timeout  = EIT_TIMEOUT;
    param.buffer   = node->buffer;
    param.buf_size = sizeof(node->buffer);
    param.oneshot  = false;

    pthread_mutex_init(&node->locker, NULL);
    pthread_mutex_init(&node->waiter, NULL);

    pthread_mutex_lock(&node->locker);
    pthread_mutex_lock(&node->waiter);

    node->fe_id = fe_id;
    node->tag   = stag_dvb_event_information_schedule_actual;

    if ((id = pthread_create(&thr, NULL, get_guide, (void*)node)) != 0) {
        print_error("epg manager thread create failed [%x]", id);
        free(node);
        return -1;
    }

    node->thread = thr;

    node->handle = ixstb_section_start(fe_id, &param, eit_callback, (void*)node);

    list_add_tail(&node->list, &guide_list_.list);

    return 0;
}

int ixsvc_epgmgr_stop(int fe_id)
{
    ixsvc_epg_local_t   *node = find_node_use_fe_id(fe_id);

    if (node) {
        void    *res;

        list_del(&node->list);

        ixstb_section_stop(node->handle);

        pthread_cancel(node->thread);
        pthread_join(node->thread, &res);

        free(node);
    }

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
