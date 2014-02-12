/* ******************************************************************************************************************
 * Copyright (c) 2013, JBSignal Co.
 * All rights reserved.
 * ******************************************************************************************************************
 * wuyong.yi@jbsignal.com / 2013.2.8.
 * ******************************************************************************************************************
 * inter-thread message passing handler
 * ****************************************************************************************************************** */

#ifndef IXSYS_MSGQ_H_
#define IXSYS_MSGQ_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <pthread.h>

/* ****************************************************************************************************************** */

typedef struct _ix_msgq_handle_t {
    pthread_mutex_t     locker;
    pthread_mutex_t     waiter;
    size_t  msg_size;
    int     max_msgs;
    int     msg_nums;
    int     msg_head;
    int     msg_tail;
    void    *msg_buf;
} *ix_msgq_handle_t;

/* ****************************************************************************************************************** */

extern ix_msgq_handle_t ixsys_msgq_create(size_t msg_size, int max_msgs);
extern int  ixsys_msgq_destroy(ix_msgq_handle_t handle);
extern int  ixsys_msgq_send(ix_msgq_handle_t handle, void *msg);
extern int  ixsys_msgq_recv(ix_msgq_handle_t handle, void *msg, int timeout);
extern int  ixsys_msgq_get_msgs(ix_msgq_handle_t handle);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IXSYS_MSGQ_H_ */

/* end of file ****************************************************************************************************** */
