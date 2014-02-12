/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsys_msgq.h"
#include "ixsys_print.h"

/* ****************************************************************************************************************** */

ix_msgq_handle_t ixsys_msgq_create(size_t msg_size, int max_msgs)
{
    ix_msgq_handle_t    handle;

    if (!msg_size || !max_msgs) {
        print_error("msgq create failed");
        return NULL;
    }

    if ((handle = (ix_msgq_handle_t)calloc(1, sizeof(struct _ix_msgq_handle_t))) == NULL) {
        print_error("msgq create failed");
        return NULL;
    }

    if ((handle->msg_buf = (void*)malloc(msg_size * max_msgs)) == NULL) {
        print_error("msgq create failed");
        return NULL;
    }

    pthread_mutex_init(&handle->locker, NULL);
    pthread_mutex_init(&handle->waiter, NULL);
    handle->msg_size = msg_size;
    handle->max_msgs = max_msgs;
    handle->msg_nums = 0;
    handle->msg_head = 0;
    handle->msg_tail = -1;

    // 메세지큐가 비었을 경우 receive시 holding되도록 하기 위해서
    pthread_mutex_lock(&handle->waiter);

    return handle;
}

int ixsys_msgq_destroy(ix_msgq_handle_t handle)
{
    pthread_mutex_lock(&handle->locker);

    if (handle->msg_nums != 0)
        print_warn("remained messages in the message queue: %d", handle->msg_nums);

    pthread_mutex_unlock(&handle->locker);

    /* release the resource */
    pthread_mutex_destroy(&handle->waiter);
    pthread_mutex_destroy(&handle->locker);
    free(handle->msg_buf);
    free(handle);

    return 0;
}

int ixsys_msgq_send(ix_msgq_handle_t handle, void *msg)
{
    if (!handle || !msg) {
        print_error("msgq or msg is not valid");
        return -1;
    }

    pthread_mutex_lock(&handle->locker);

    if (handle->msg_nums == handle->max_msgs) {
        print_error("msgq is full");

        pthread_mutex_unlock(&handle->locker);
        return -1;
    }

    // 최초 메세지 삽입시 receive의 hold를 풀어주도록
    if (handle->msg_nums == 0)
        pthread_mutex_unlock(&handle->waiter);

    if (handle->msg_tail == (handle->max_msgs - 1))
        handle->msg_tail = 0;
    else
        ++handle->msg_tail;

    memcpy(handle->msg_buf + (handle->msg_tail * handle->msg_size), msg, handle->msg_size);
    ++handle->msg_nums;

    pthread_mutex_unlock(&handle->locker);

    return 0;
}

int ixsys_msgq_recv(ix_msgq_handle_t handle, void *msg, int timeout)
{
    if (!handle || !msg) {
        print_error("msgq or msg is not valid");
        return -1;
    }

    /* 메세지큐가 비었을 경우, 기다림 */
    if (timeout == 0) {
        pthread_mutex_lock(&handle->waiter);
    } else {
        while (pthread_mutex_trylock(&handle->waiter) != 0) {
            if (timeout-- == 0) {
                print_error("msgq is empty - timeout");
                return -1;
            }
            usleep(1000);
        }
    }

    pthread_mutex_lock(&handle->locker);

    memcpy(msg, handle->msg_buf + (handle->msg_head * handle->msg_size), handle->msg_size);
    --handle->msg_nums;

    ++handle->msg_head;
    if (handle->msg_head == handle->max_msgs)
        handle->msg_head = 0;

    // 메세지큐에 메세지가 있는 경우에만 다시 진입할 수 있도록 준비
    if (handle->msg_nums)
        pthread_mutex_unlock(&handle->waiter);

    pthread_mutex_unlock(&handle->locker);

    return 0;
}

int ixsys_msgq_get_msgs(ix_msgq_handle_t handle)
{
    if (!handle) {
        print_error("msgq is not valid");
        return -1;
    }

    int     msgs;

    pthread_mutex_lock(&handle->locker);

    msgs = handle->msg_nums;

    pthread_mutex_unlock(&handle->locker);

    return msgs;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
