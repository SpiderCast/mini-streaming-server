/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_section.h"
#include "ixsys_print.h"
#include <errno.h>
#include <sys/poll.h>
#include "ixstb_dmx.h"

/* ****************************************************************************************************************** */

typedef struct _ixstb_section_signal_t {
    int         dmxfd;
    uint8_t     *buffer;
    size_t      buf_size;
    int         timeout;
    bool        oneshot;
    void        *private;
    ix_section_cb   cb;
} ixstb_section_signal_t;

/* ****************************************************************************************************************** */

static void* get_section(void *param)
{
    ixstb_section_signal_t  *sig = (ixstb_section_signal_t*)param;
    struct pollfd       pfd;
    int     len = 0;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    while (1) {
        memset(&pfd, 0, sizeof(pfd));
        pfd.fd      = sig->dmxfd;
        pfd.events  = POLLIN;
        pfd.revents = 0;

        if (poll(&pfd, 1, sig->timeout) < 0) {
            (sig->cb)(0, sig->private);
            return NULL;
        }

        if (pfd.revents) {
            if (((len = read(sig->dmxfd, sig->buffer, sig->buf_size)) < 0) && (errno == EOVERFLOW)) {
                len = read(sig->dmxfd, sig->buffer, sig->buf_size);
            }
        }

        if (len < 0)
            (sig->cb)(0, sig->private);
        else
            (sig->cb)(len, sig->private);

        if (sig->oneshot == true)
            break;
    }

    return NULL;
}

ix_section_handle_t ixstb_section_start(int fe_id, ix_section_param_t *param, ix_section_cb cb, void *priv)
{
    int         dmxfd, id;
    pthread_t   thr;
    ix_section_handle_t     handle;
    ixstb_section_signal_t  *sig;

    if ((dmxfd = ixstb_dmx_set_section_filter(fe_id, param->pid, param->f_bytes, param->f_masks, param->f_size, param->buf_size, param->timeout, param->oneshot)) < 0) {
        print_error("section filter error");
        return NULL;
    }

    sig = (ixstb_section_signal_t*)calloc(1, sizeof(ixstb_section_signal_t));

    sig->dmxfd    = dmxfd;
    sig->buffer   = param->buffer;
    sig->buf_size = param->buf_size;
    sig->timeout  = param->timeout;
    sig->oneshot  = param->oneshot;
    sig->private  = priv;
    sig->cb       = cb;

    if ((id = pthread_create(&thr, NULL, get_section, (void*)sig)) != 0) {
        print_error("gather section create failed [%x]", id);
        ixstb_dmx_clr_filter(dmxfd);
        free(sig);
        return NULL;
    }

    ixstb_dmx_start_filter(dmxfd);

    handle = (ix_section_handle_t)calloc(1, sizeof(struct _ix_section_handle_t));

    handle->thread = thr;
    handle->dmxfd  = dmxfd;
    handle->signal = (void*)sig;

    return handle;
}

int ixstb_section_stop(ix_section_handle_t handle)
{
    if (handle) {
        void    *res;

        ixstb_dmx_clr_filter(handle->dmxfd);

        pthread_cancel(handle->thread);
        pthread_join(handle->thread, &res);

        free(handle->signal);
        free(handle);
    }

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
