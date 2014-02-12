/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsys_print.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "ix_jsonop.h"
#include "ixsys_sysinfo.h"
#include "ixsvc_findme.h"

/* ****************************************************************************************************************** */

enum { RECV_BUF_SIZE = 512 };

/* ****************************************************************************************************************** */

static pthread_t    findme_handle_;

/* ****************************************************************************************************************** */

static int responser(char *data, ssize_t size, struct sockaddr_in *remote_addr, socklen_t addr_len)
{
    cJSON   *json = cJSON_Parse(data);
    cJSON   *node;
    int     sock;
    char    ipstr[16];
    char    *out;

    if (!json) {
        print_error("illegal data");
        return -1;
    }

    if ((node = cJSON_GetObjectItem(json, MOBI_WHERE_ARE_YOU)) == NULL) {
        print_error("illegal data");
        cJSON_Delete(json);
        return -1;
    }

    cJSON_Delete(json);

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        print_error("socket open error");
        return -1;
    }

    json = cJSON_CreateObject();
    cJSON_AddItemToObject(json, BOX_IM_HERE, node = cJSON_CreateObject());
    ixsys_get_ipaddr(NULL, ipstr, sizeof(ipstr));
    cJSON_AddItemToObject(node, BOX_IM_HERE_MY_IP, cJSON_CreateString(ipstr));

    out = cJSON_Print(json);

    remote_addr->sin_port = htons(1124);
    sendto(sock, out, strlen(out), 0, (struct sockaddr*)remote_addr, addr_len);

    free(out);
    cJSON_Delete(json);

    close(sock);

    return 0;
}

static void* findme(void *param)
{
    int         sock;
    ssize_t     n_recv;
    char        recv_buf[RECV_BUF_SIZE];
    socklen_t   addr_size;
    struct sockaddr_in  local_addr, remote_addr;

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        print_error("socket open error");
        return NULL;
    }

    bzero(&local_addr, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port   = htons(1123);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) == -1) {
        print_error("socket bind error");
        close(sock);
        return NULL;
    }

    while (1) {
        if ((n_recv = recvfrom(sock, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr*)&remote_addr, &addr_size)) > 0)
            responser(recv_buf, n_recv, &remote_addr, addr_size);
    }

    close(sock);

    return NULL;
}

int ixsvc_findme_init()
{
    int     id;

    if ((id = pthread_create(&findme_handle_, NULL, findme, NULL)) != 0) {
        print_error("findme create failed [%x]", id);
        return -1;
    }

    return 0;
}

#if 0
// test code

enum { RECV_BUF_SIZE = 512 };

#define SAMPLE_JSON     "{          \
    \"whereau\": {                  \
        \"ip\": \"client IP\"       \
    }                               \
}"

int main()
{
    int         s1, s2;
    ssize_t     n_recv;
    int         bcast_on = 1;
    char        recv_buf[RECV_BUF_SIZE];
    socklen_t   addr_size;
    struct sockaddr_in  local_addr, remote_addr;

    if ((s1 = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("socket open error\n");
        return 0;
    }
    printf("%d:%d\n", __LINE__, setsockopt(s1, SOL_SOCKET, SO_BROADCAST, (void*)&bcast_on, sizeof(bcast_on)));

    bzero(&remote_addr, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port   = htons(1123);
    remote_addr.sin_addr.s_addr = INADDR_BROADCAST;

    printf("%d:%ld\n", __LINE__, sendto(s1, SAMPLE_JSON, strlen(SAMPLE_JSON), 0, (struct sockaddr*)&remote_addr, sizeof(remote_addr)));
    close(s1);

    remote_addr.sin_port = htons(1124);
    remote_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s2 = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (bind(s2, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) == -1) {
        printf("socket bind error\n");
        close(s2);
        return 0;
    }

    n_recv = recvfrom(s2, recv_buf, RECV_BUF_SIZE, 0, (struct sockaddr*)&remote_addr, &addr_size);

    printf("recv_buf: %s\n", recv_buf);

    close(s2);

    return 0;
}
#endif

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
