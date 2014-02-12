/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsvc_listener.h"
#include "ixsys_print.h"
#include <sys/un.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "ixsvc_cmd_parser.h"

/* ****************************************************************************************************************** */

enum { RECV_BUF_SIZE = 2048 };

/* ****************************************************************************************************************** */

static pthread_t    listener_handle_;

/* ****************************************************************************************************************** */

static int pre_parser(char *data, ssize_t size)
{
    print_dump("## LISTEN ##", (uint8_t*)data, size);
    /* decoding, descramble, unzip, ... */

    return size;
}

static void* listener(void *param)
{
    int         sock;
    ssize_t     n_recv;
    char        recv_buf[RECV_BUF_SIZE];
    char        *out = NULL;
    socklen_t   addr_size;
    struct sockaddr_un  local_addr, remote_addr;

    if (access(CGI_TO_STB_PATH, F_OK) == 0)
        unlink(CGI_TO_STB_PATH);

    if ((sock = socket(PF_FILE, SOCK_DGRAM, 0)) == -1) {
        print_error("socket open error");
        return NULL;
    }

    bzero(&local_addr, sizeof(local_addr));
    local_addr.sun_family = AF_FILE;
    strcpy(local_addr.sun_path, CGI_TO_STB_PATH);

    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) == -1) {
        print_error("socket bind error");
        close(sock);
        return NULL;
    }

    addr_size = sizeof(remote_addr);

    while (1) {
        if ((n_recv = recvfrom(sock, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr*)&remote_addr, &addr_size)) > 0) {
            // print_debug("### %s ###\n", remote_addr.sun_path);
            if ((n_recv = pre_parser(recv_buf, n_recv)) > 0) {
                if ((out = ixsvc_cmd_parser(recv_buf, n_recv)) != NULL) {
                    sendto(sock, out, strlen(out), 0, (struct sockaddr*)&remote_addr, addr_size);
                    free(out);
                }
            }
        }
    }

    close(sock);

    return NULL;
}

int ixsvc_listener_init()
{
    int     id;

    if ((id = pthread_create(&listener_handle_, NULL, listener, NULL)) != 0) {
        print_error("listener create failed [%x]", id);
        return -1;
    }

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
