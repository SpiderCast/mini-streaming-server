/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixcgi_makepage.h"
#include "ix_jsonop.h"

/* ****************************************************************************************************************** */

typedef cJSON*  (*ixcgi_errpage_proc)();

/* ****************************************************************************************************************** */

static cJSON* error_page_stbd_timeout()
{
    cJSON   *root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, ERROR_CODE, cJSON_CreateString(ERROR_CODE_TIMEOUT));
    cJSON_AddItemToObject(root, ERROR_CODE_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));

    return root;
}

static cJSON* error_page_db_busy()
{
    cJSON   *root;

    if ((root = cJSON_CreateObject()) == NULL)
        return NULL;

    cJSON_AddItemToObject(root, ERROR_CODE, cJSON_CreateString(ERROR_CODE_DB_BUSY));
    cJSON_AddItemToObject(root, ERROR_CODE_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));

    return root;
}

static cJSON* error_page_illegal_data()
{
    cJSON   *root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, ERROR_CODE, cJSON_CreateString(ERROR_CODE_ILLEGAL));
    cJSON_AddItemToObject(root, ERROR_CODE_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));

    return root;
}

static cJSON* error_page_unknown()
{
    cJSON   *root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, ERROR_CODE, cJSON_CreateString(ERROR_CODE_UNKNOWN));
    cJSON_AddItemToObject(root, ERROR_CODE_MY_IP, cJSON_CreateString(getenv("HTTP_HOST")));

    return root;
}

/* ****************************************************************************************************************** */

static ixcgi_errpage_proc   error_pages[] = {
    error_page_stbd_timeout,    /* _error_stbd_timeout */
    error_page_db_busy,         /* _error_db_busy */
    error_page_illegal_data,    /* _error_illegal_data */
    error_page_unknown          /* _error_unknown */
};

/* ****************************************************************************************************************** */

cJSON* ixcgi_make_error_page(ixcgi_error_t err)
{
    return (error_pages[err])();
}

void ixcgi_make_raw_page(char *out)
{
    printf("content-type: text/plain;charset=UTF-8\n\n");
    printf("%s\n\n", out);
}

int ixcgi_make_text_page(cJSON *json)
{
    char    *out = cJSON_Print(json);

    printf("content-type: text/plain;charset=UTF-8\n\n");
    printf("%s\n\n", out);

    free(out);

    return 0;
}

int ixcgi_make_html_page(cJSON *json)
{
    char    *out = cJSON_Print(json);

    printf("content-type: text/plain;charset=UTF-8\n\n");
    printf("%s\n\n", out);

    free(out);

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
