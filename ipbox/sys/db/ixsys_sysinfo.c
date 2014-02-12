/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsys_sysinfo.h"
#include "ixsys_print.h"
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>

/* ****************************************************************************************************************** */

uint64_t ixsys_get_model(char *str, size_t len)
{
    if (str) {
        memset(str, 0, len);
        snprintf(str, len - 1, "jIPBOX-000");
    }

    return 0;
}

uint64_t ixsys_get_hwver(char *str, size_t len)
{
    if (str) {
        memset(str, 0, len);
        snprintf(str, len - 1, "jIPBOX-20120801 rev.01");
    }

    return 0;
}

uint64_t ixsys_get_swver(char *str, size_t len)
{
    if (str) {
        memset(str, 0, len);
        snprintf(str, len - 1, "jIPBOX-20120222 rev.1234");
    }

    return 0;
}

int ixsys_get_ipaddr(uint64_t *ipaddr, char *str, size_t len)
{
    int     sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq    ifr;
    struct sockaddr_in  *sin;

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, "eth0");

    if (ioctl(sock, SIOCGIFADDR, &ifr) == 0) {
        sin = (struct sockaddr_in*)&ifr.ifr_addr;

        if (ipaddr)
            *ipaddr = ifr.ifr_addr.sa_data[2] << 24 | ifr.ifr_addr.sa_data[3] << 16 | ifr.ifr_addr.sa_data[4] << 8 | ifr.ifr_addr.sa_data[5];
        if (str)
            strcpy(str, inet_ntoa(sin->sin_addr));

        close(sock);
        return 4;
    }

    close(sock);
    return 0;
}

uint64_t ixsys_get_macaddr(char *str, size_t len)
{
    int     sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq    ifr;
    uint64_t        macaddr = 0;

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, "eth0");

    if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
        macaddr = (uint64_t)ifr.ifr_hwaddr.sa_data[0] << 40 | (uint64_t)ifr.ifr_hwaddr.sa_data[1] << 32 | ifr.ifr_hwaddr.sa_data[2] << 24
                  | ifr.ifr_hwaddr.sa_data[3] << 16 | ifr.ifr_hwaddr.sa_data[4] << 8 | ifr.ifr_hwaddr.sa_data[5];

        if (str) {
            memset(str, 0, len);
            snprintf(str, len - 1, "%02x:%02x:%02x:%02x:%02x:%02x"
                                 , ifr.ifr_hwaddr.sa_data[0], ifr.ifr_hwaddr.sa_data[1], ifr.ifr_hwaddr.sa_data[2]
                                 , ifr.ifr_hwaddr.sa_data[3], ifr.ifr_hwaddr.sa_data[4], ifr.ifr_hwaddr.sa_data[5]);
        }
    }

    close(sock);
    return macaddr;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
