/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsys_print.h"
#include <pthread.h>

/* ****************************************************************************************************************** */

static pthread_mutex_t  log_mutex_lock_ = PTHREAD_MUTEX_INITIALIZER;
static char             log_msg_buf_[256];

/* ****************************************************************************************************************** */

void ixsys_print_init()
{
}

void ixsys_print(int err_flag, char *format, ...)
{
    va_list         args;

    pthread_mutex_lock(&log_mutex_lock_);

    va_start(args, format);
    vsprintf(log_msg_buf_, (char*)format, args);
    va_end(args);

    if (err_flag)
        fprintf(stderr, "%s", log_msg_buf_);
    else
        printf("%s", log_msg_buf_);

    pthread_mutex_unlock(&log_mutex_lock_);
}

void ixsys_print_dump(char *title, uint8_t *data, size_t size)
{
    int     n;
    char    str[12], line[80], txt[28];
    uint8_t *p;

    if (!data || !size)
        return;

    ixsys_print(0, "[%s|%u]\n", title, size);

    n = 0;
    p = data;
    line[0] = '\0';

    while ((uint16_t)(p - data) < size) {
        sprintf(str, "%02x ", *p);
        strcat(line, str);
        txt[n++] = ((*p >= 0x20) && (*p < 0x7f)) ? *p : '.';

        if (n == 16) {
            txt[n] = '\0';
            ixsys_print(0, "%s %s\n", line, txt);
            n = 0;
            line[0] = '\0';
        }

        ++p;
    }

    if (n) {
        txt[n] = '\0';

        do {
            strcat(line, "   ");
        } while (++n < 16);

        ixsys_print(0, "%s %s\n", line, txt);
    }
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
