/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixsys_print.h"
#include <pthread.h>
#include "ixsys_db.h"
#include "stb/ixstb_fe.h"
#include "stb/ixstb_descr.h"
#include "stb/ixstb_fakepat.h"
#include "svc/ixsvc_listener.h"
#include "svc/ixsvc_findme.h"
#include "svc/ixsvc_epgmgr.h"

/* ****************************************************************************************************************** */

static void early_init()
{
    system("rm -f /var/www/streaming/*.ts");
    ixsys_print_init();
}

static void system_init()
{
    ixsys_db_init();
    ixstb_fe_init();
    ixstb_descr_init();
    ixstb_fakepat_init();
    ixsvc_listener_init();
    ixsvc_findme_init();
    ixsvc_epgmgr_init();
}

int main(int argc, const char *argv[])
{
    early_init();
    system_init();

    print_debug("###################################################################\n");
    print_debug("## program starterd ............................................ ##\n");
    print_debug("###################################################################\n");

    /* never this */
    pthread_mutex_t  never_this = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&never_this);
    pthread_mutex_lock(&never_this);

    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
