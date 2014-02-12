/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixcgi_getenv.h"
#include "ixcgi_urlenc.h"

/* ****************************************************************************************************************** */

/**
 * 리턴값에 대해서 반드시 free를 해줘야 한다.
 */
char* ixcgi_get_query_string(char *key)
{
    char    *q = getenv("QUERY_STRING");
    char    *query, *c, *p;
    size_t  len = 0, key_len = strlen(key);

    if (!q)
        return NULL;

    query = (char*)malloc(strlen(q) + 1);
    memset(query, 0, strlen(q) + 1);

    ixcgi_urldec(q, query);
    c = query;

    while ((c = strstr(c, key))) {
        if (((c == query) || *(c - 1) == '&') && ((*(c + key_len) == '=') || (*(c + key_len) == '\0'))) {
            p = c = c + strlen(key) + 1;

            /* value의 길이 계산 */
            while ((++len) && (*p != '\0') && (*p != '&'))
                ++p;

            p = (char*)malloc(len);
            memset(p, 0, len);
            strncpy(p, c, len - 1);

            free(query);
            return p;
        }

        c += strlen(key);
    }

    free(query);
    return NULL;
}

/**
 * 리턴값에 대해서 반드시 free를 해줘야 한다.
 */
char* ixcgi_get_user_agent()
{
    char    *query = getenv("HTTP_USER_AGENT");
    char    *p;
    size_t  len = 0;

    if (!query)
        return NULL;

    len = strlen(query) + 1;
    p = (char*)malloc(len);
    memset(p, 0, len);
    strcpy(p, query);

    return p;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
