/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixcgi_urlenc.h"
#include <ctype.h>

/* ****************************************************************************************************************** */

static int decode_hton(char *src)
{
    int     n, num = 0, val = 0;

    for (n = 0; n < 2; n++) {
        if ((*src >= '0') && (*src <= '9')) {
            num = *src - '0';
        } else if ((*src >= 'A') && (*src <= 'Z')) {
            num = (*src - 'A') + 10;
        } else if ((*src >= 'a') && (*src <= 'z')) {
            num = (*src - 'a') + 10;
        }

        if (16 * (1 - n))
            num *= 16;
        val += num;

        ++src;
    }

    return val;
}

int ixcgi_urlenc(char *src, char *dst)
{
    char    hex[4];
    int     size = 0;

    while (*src) {
        if (isalnum(*src) || (*src == '-') || (*src == '.') || (*src == '_')) {
            *dst = *src;
        } else {
            snprintf(hex, 4, "%%%02x", *src);
            strncat(dst, hex, 3);
            dst += 2;
            size += 2;
        }

        ++src;
        ++dst;
        ++size;
    }

    return size;
}

int ixcgi_urldec(char *src, char *dst)
{
    int     n = 0;

    while (*src) {
        if (*src == '%') {
            ++src;
            dst[n] = decode_hton(src);
            ++src;
        } else {
            dst[n] = *src;
        }

        if (dst[n] == '%') {
            ++src;
            dst[n] = decode_hton(src);
            ++src;
        }

        ++src;
        ++n;
    }

    return n;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
