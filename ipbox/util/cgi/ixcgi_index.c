/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include "ix_jsonop.h"
#include "ixcgi_getenv.h"
#include "ixcgi_opcode_parser.h"
#include "ixcgi_makepage.h"

/* ****************************************************************************************************************** */

int main(int argc, const char *argv[])
{
    char    *ua = getenv("HTTP_USER_AGENT");
    char    *value = ixcgi_get_query_string(HTTPD_OPCODE);
    cJSON   *json;

    if (value) {
        json = ixcgi_opcode_parser(value);
        free(value);
    } else {
        json = ixcgi_opcode_parser(JOP_SYSTEM_INFO);
    }

    if (strncmp(ua, IXCGI_CUSTOMAPP_UA, strlen(IXCGI_CUSTOMAPP_UA)) == 0) {
        ixcgi_make_text_page(json);
    } else {
        ixcgi_make_html_page(json);
    }

    cJSON_Delete(json);

    // printf("HTTP_USER_AGENT: %s\n", getenv("HTTP_USER_AGENT"));
    // printf("SERVER_SOFTWARE: %s\n", getenv("SERVER_SOFTWARE"));
    // printf("REMOTE_ADDR: %s\n", getenv("REMOTE_ADDR"));
    // printf("HTTP_HOST: %s\n", getenv("HTTP_HOST"));

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
