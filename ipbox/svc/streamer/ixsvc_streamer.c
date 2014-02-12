/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsvc_streamer.h"
#include "ixsys_print.h"
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libucsi/transport_packet.h>
#include "stb/ixstb_descr.h"
#include "stb/ixstb_fakepat.h"
#include "ixsys_sysinfo.h"

/* ****************************************************************************************************************** */

#define STREAMER_BUFFER_SIZE        (188 * 5000)

typedef struct _ixsvc_streamer_local_t {
    ix_streaming_t  type;

    pthread_t   thread;

    bool        is_recording;
    bool        is_finished;

    int         fd;

    ssize_t     sent_bytes;
    time_t      duration_sec;

    int         sock;

    /* for streaming */
    uint16_t    port;
    int         ttl;
    struct sockaddr_in  addr;
} ixsvc_streamer_local_t;

/* ****************************************************************************************************************** */

static void blackbox(uint8_t *data, ssize_t data_len)
{
#if 0
#if 0
    int     n, pid;
    struct transport_packet *ts_packet;

    for (n = 0; n < data_len; n += 188) {
        if ((ts_packet = transport_packet_init(data + n)) == NULL)
            continue;

        pid = transport_packet_pid(ts_packet);
        switch (pid) {
        case 121:
            ixstb_descr_packets(121, data + n, 188);
            break;
        case 131:
            ixstb_descr_packets(131, data + n, 188);
            break;
        }
    }
#else
    int     n;

    for (n = 0; n < data_len; n += 188) {
        ixstb_descr_packets(131, data + n, 188);
    }
#endif
#endif
}

static void* streamer(void *param)
{
    ixsvc_streamer_local_t  *local = (ixsvc_streamer_local_t*)param;
    struct timespec         ts;
    time_t      start_time;
    uint8_t     buffer[STREAMER_BUFFER_SIZE + 188];
    ssize_t     length;

// uint8_t key[] = { 0x11, 0x11, 0x11, 0x33, 0x11, 0x11, 0x11, 0x33 };
// ixstb_descr_alloc_key(131, key, key);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    clock_gettime(CLOCK_REALTIME, &ts);
    start_time = ts.tv_sec;

    memcpy(buffer, ixstb_fakepat_get_packet(local->fd), 188);

    while (1) {
        if ((length = read(local->fd, buffer + 188, STREAMER_BUFFER_SIZE)) > 0) {

            blackbox(buffer + 188, length);

            if ((local->is_recording) || (local->type == _streaming_http))  {
                length = write(local->sock, buffer, length + 188);
            } else {
                length = sendto(local->sock, buffer, length + 188, 0, (struct sockaddr*)&local->addr, sizeof(local->addr));
            }

            print_debug("writing or sending: %d\n", length);

            clock_gettime(CLOCK_REALTIME, &ts);
            local->duration_sec = ts.tv_sec - start_time;
        }
    }

    local->is_finished = true;

    return NULL;
}

ix_streamer_handle_t ixsvc_streamer_request(ix_streaming_t type, char *ip, uint16_t port, int ttl, int fd, char *uri)
{
    ix_streamer_handle_t    handle;
    ixsvc_streamer_local_t  *local;
    struct sockaddr_in      mc_addr;
    int         sock;
    pthread_t   thr;
    int         n;

    if (type == _streaming_http) {
        if (access(uri, F_OK) == 0)
            unlink(uri);

        if (mkfifo(uri, 0666) < 0) {
            print_error("fifo create error");
            return NULL;
        }

        if ((sock = open(uri, O_RDWR)) < 0) {
            print_error("fifo file open error: %s", uri);
            return NULL;
        }

        print_debug("streaming HTTP : %s : %d : %d : %s\n", ip, port, ttl, uri);
    } else {
        if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
            print_error("socket() : %d", errno);
            return NULL;
        }

        bzero(&mc_addr, sizeof(mc_addr));
        mc_addr.sin_family = AF_INET;
        mc_addr.sin_port   = htons(port);
        mc_addr.sin_addr.s_addr = inet_addr(ip);

        if (type == _streaming_multicast) {
            char    loopback = 0;
            char    src_ip[16];
            struct in_addr  local_if;

            /* disable loopback */

            if ((n = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (void*)&loopback, sizeof(loopback))) < 0) {
                print_error("setsockopt() : %d", errno);
                close(sock);
                return NULL;
            }

            /* set local interface for outbound multicast datagram */

            ixsys_get_ipaddr(NULL, src_ip, sizeof(src_ip));
            local_if.s_addr = inet_addr(src_ip);

            if ((n = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (void*)&local_if, sizeof(local_if))) < 0) {
                print_error("setsockopt() : %d", errno);
                close(sock);
                return NULL;
            }
        }

        if ((n = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&ttl, sizeof(ttl))) < 0) {
            print_error("setsockopt() : %d", errno);
            close(sock);
            return NULL;
        }
    }

    local = (ixsvc_streamer_local_t*)calloc(1, sizeof(ixsvc_streamer_local_t));

    local->type = type;
    local->is_recording = false;
    local->is_finished  = false;
    local->sock = sock;
    local->port = port;
    local->ttl  = ttl;
    local->addr = mc_addr;
    local->fd   = fd;

    if ((n = pthread_create(&thr, NULL, streamer, (void*)local)) != 0) {
        print_error("streamer create failed [%x]", n);
        close(sock);
        free(local);
        return NULL;
    }

    local->thread = thr;

    handle = (ix_streamer_handle_t)calloc(1, sizeof(struct _ix_streamer_handle_t));
    handle->local = local;

    return handle;
}

int ixsvc_streamer_retract(ix_streamer_handle_t handle, char *uri)
{
    if (handle) {
        void    *res;
        ixsvc_streamer_local_t  *local = handle->local;

        if (local->type == _streaming_http) {
            if (access(uri, F_OK) == 0)
                unlink(uri);
        }

        pthread_cancel(local->thread);
        pthread_join(local->thread, &res);

        close(local->sock);

        free(local);
        free(handle);
    }

    return 0;
}

int ixsvc_streamer_get_duration_info(ix_streamer_handle_t handle, ssize_t *sent_bytes, int *duration_sec)
{
    if (handle) {
        ixsvc_streamer_local_t  *local = handle->local;

        *sent_bytes   = local->sent_bytes;
        *duration_sec = (int)local->duration_sec;

        return 0;
    }

    *sent_bytes   = 0;
    *duration_sec = 0;

    return -1;
}

ix_recorder_handle_t ixsvc_recorder_request(char *path, int fd)
{
    ix_streamer_handle_t    handle;
    ixsvc_streamer_local_t  *local;
    pthread_t   thr;
    int         n;

    local = (ixsvc_streamer_local_t*)calloc(1, sizeof(ixsvc_streamer_local_t));

    local->is_recording = true;
    local->is_finished  = false;
    local->fd           = fd;

    if ((local->sock = open(path, O_CREAT|O_RDWR|O_NONBLOCK)) < 0) {
        print_error("recording file open error: %s", path);
        free(local);
        return NULL;
    }

    if ((n = pthread_create(&thr, NULL, streamer, (void*)local)) != 0) {
        print_error("recorder create failed [%x]", n);
        close(local->sock);
        free(local);
        return NULL;
    }

    local->thread = thr;

    handle = (ix_streamer_handle_t)calloc(1, sizeof(struct _ix_streamer_handle_t));
    handle->local = local;

    return handle;
}

int ixsvc_recorder_retract(ix_recorder_handle_t handle)
{
   return ixsvc_streamer_retract(handle, NULL);
}

int ixsvc_recorder_get_duration_info(ix_recorder_handle_t handle, ssize_t *written_bytes, int *duration_sec)
{
    return ixsvc_streamer_get_duration_info(handle, written_bytes, duration_sec);
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
