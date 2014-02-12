/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_dmx.h"
#include "ixsys_print.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include "ixstb_fe.h"

/* ****************************************************************************************************************** */

int ixstb_dmx_set_section_filter(int fe_id, uint16_t pid, uint8_t *f_bytes, uint8_t *f_masks, size_t f_len, size_t buf_size, int timeout, bool oneshot)
{
    struct dmx_sct_filter_params    filter;
    int     fd;
    int16_t adapter, frontend;
    char    path[64];

    if (ixstb_fe_get_devid(fe_id, &adapter, &frontend) == -1) {
        print_error("illegal frontend id: %d", fe_id);
        return -1;
    }

    memset(path, 0, sizeof(path));
    snprintf(path, sizeof(path), "/dev/dvb/adapter%d/demux%d", adapter, frontend);

    if ((fd = open(path, O_RDWR|O_NONBLOCK)) < 0) {
        print_error("demux open error: %s", path);
        return -1;
    }

    memset(&filter, 0, sizeof(filter));
    filter.pid     = pid;
    filter.timeout = timeout;
    filter.flags   = DMX_CHECK_CRC | (oneshot ? DMX_ONESHOT : 0);
    memcpy(filter.filter.filter, f_bytes, f_len);
    memcpy(filter.filter.mask, f_masks, f_len);

    if (ioctl(fd, DMX_SET_FILTER, &filter) < 0) {
        print_error("set filter error");
        close(fd);
        return -1;
    }

    if (buf_size > 0) {
        if (ioctl(fd, DMX_SET_BUFFER_SIZE, buf_size) < 0) {
            print_error("set buffer size error: %d", buf_size);
            close(fd);
            return -1;
        }
    }

    return fd;
}

int ixstb_dmx_set_pid_filter(int fe_id, uint16_t pid, size_t buf_size, dmx_input_t input, dmx_output_t output, dmx_pes_type_t pes_type)
{
    struct dmx_pes_filter_params    filter;
    int     fd;
    int16_t adapter, frontend;
    char    path[64];

    if (ixstb_fe_get_devid(fe_id, &adapter, &frontend) == -1) {
        print_error("illegal frontend id: %d", fe_id);
        return -1;
    }

    memset(path, 0, sizeof(path));
    snprintf(path, sizeof(path), "/dev/dvb/adapter%d/demux%d", adapter, frontend);

    if ((fd = open(path, O_RDWR|O_NONBLOCK)) < 0) {
        print_error("demux open error: %s", path);
        return -1;
    }

    memset(&filter, 0, sizeof(filter));
    filter.pid      = pid;
    filter.input    = input;
    filter.output   = output;
    filter.pes_type = pes_type;
    filter.flags    = 0;

    if (ioctl(fd, DMX_SET_PES_FILTER, &filter) < 0) {
        print_error("set filter error");
        close(fd);
        return -1;
    }

    if (buf_size > 0) {
        if (ioctl(fd, DMX_SET_BUFFER_SIZE, buf_size) < 0) {
            print_error("set buffer size error: %d", buf_size);
            close(fd);
            return -1;
        }
    }

    return fd;
}

int ixstb_dmx_clr_filter(int fd)
{
    close(fd);
    return 0;
}

int ixstb_dmx_start_filter(int fd)
{
    if (ioctl(fd, DMX_START) < 0) {
        print_error("dmx start error: %d", fd);
        return -1;
    }

    return 0;
}

int ixstb_dmx_stop_filter(int fd)
{
    if (ioctl(fd, DMX_STOP) < 0) {
        print_error("dmx stop error: %d", fd);
        return -1;
    }

    return 0;
}

int ixstb_dmx_open_dvr(int fe_id, size_t buf_size)
{
    int     fd;
    int16_t adapter, frontend;
    char    path[64];

    if (ixstb_fe_get_devid(fe_id, &adapter, &frontend) == -1) {
        print_error("illegal frontend id: %d", fe_id);
        return -1;
    }

    memset(path, 0, sizeof(path));
    snprintf(path, sizeof(path), "/dev/dvb/adapter%d/dvr%d", adapter, frontend);

    if ((fd = open(path, O_RDONLY)) < 0) {
        print_error("dvr open error: %s", path);
        return -1;
    }

    if (buf_size > 0) {
        if (ioctl(fd, DMX_SET_BUFFER_SIZE, buf_size) < 0) {
            print_error("set buffer size error: %d", buf_size);
            close(fd);
            return -1;
        }
    }

    return fd;
}

int ixstb_dmx_close_dvr(int fd)
{
    close(fd);
    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
