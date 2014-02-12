/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#ifndef IXSTB_SECTION_H_
#define IXSTB_SECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <pthread.h>
#include <semaphore.h>

/* ****************************************************************************************************************** */

typedef void    (*ix_section_cb)(size_t read_size, void *priv);

typedef struct _ix_section_handle_t {
    pthread_t   thread;
    int         dmxfd;
    void        *signal;
} *ix_section_handle_t;

typedef struct _ix_section_param_t {
    uint16_t    pid;
    uint8_t     *f_bytes, *f_masks;
    size_t      f_size;
    int         timeout;
    uint8_t     *buffer;
    size_t      buf_size;
    bool        oneshot;
} ix_section_param_t;

/* ****************************************************************************************************************** */

extern ix_section_handle_t ixstb_section_start(int fe_id, ix_section_param_t *param, ix_section_cb cb, void *priv);
extern int  ixstb_section_stop(ix_section_handle_t handle);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSTB_SECTION_H_ */

/* end of file ****************************************************************************************************** */
