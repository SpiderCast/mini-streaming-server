/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsys_getstr.h"
#include "ixsys_print.h"
#include <iconv.h>
#include <libucsi/dvb/types.h>

/* ****************************************************************************************************************** */

int ixsys_iconv_dvb_to_utf8(char *dvb_str, char *utf_str, size_t str_size)
{
    size_t      dvb_strlen = strlen(dvb_str);
    size_t      utf_strlen = str_size - 1;
    int         consumed = 0;
    const char  *iso_code = dvb_charset(dvb_str, dvb_strlen, &consumed);

    memset(utf_str, 0, str_size);

    iso_code = dvb_charset(dvb_str, dvb_strlen, &consumed);

    if (consumed == 0) {
        strncpy(utf_str, dvb_str, utf_strlen);
    } else {
        iconv_t     conv;

        if ((conv = iconv_open("UTF-8", iso_code)) == (iconv_t)-1) {
            strncpy(utf_str, dvb_str + consumed, utf_strlen);
        } else {
            char    *in = dvb_str + consumed;
            char    *out = utf_str;
            size_t  in_bytes = dvb_strlen - consumed;
            size_t  out_bytes = utf_strlen;

            dvb_strlen -= consumed;

            if (iconv(conv, &in, &in_bytes, &out, &out_bytes) == -1) {
                memset(utf_str, 0, str_size);
                strncpy(utf_str, dvb_str + consumed, utf_strlen);
            }

            iconv_close(conv);
        }
    }

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
