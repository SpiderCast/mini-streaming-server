/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ixstb_descr.h"
#include "ixsys_print.h"
#include <FFdecsa/FFdecsa.h>
#include "ix_list.h"

/* ****************************************************************************************************************** */

#define CLUSTER_LIMIT   10

/* ****************************************************************************************************************** */

typedef struct _ixstb_descr_t {
    struct list_head    list;

    uint16_t    pid;
    void*       keys;

#define DESCR_KEY_LEN   8
    uint8_t     even[DESCR_KEY_LEN];
    uint8_t     odd[DESCR_KEY_LEN];
} ixstb_descr_t;

/* ****************************************************************************************************************** */

static ixstb_descr_t    descr_list_;
static void*    test_key;

/* ****************************************************************************************************************** */

static inline ixstb_descr_t* find_pid(uint16_t pid)
{
    ixstb_descr_t   *pos;

    list_for_each_entry(pos, &descr_list_.list, list) {
        if (pid == pos->pid) {
            return pos;
        }
    }

    return NULL;
}

int ixstb_descr_init()
{
    INIT_LIST_HEAD(&descr_list_.list);
    return 0;
}

int ixstb_descr_alloc_key(uint16_t pid, uint8_t *even, uint8_t *odd)
{
    ixstb_descr_t   *descr = find_pid(pid);

    if (!descr) {
        descr = (ixstb_descr_t*)calloc(1, sizeof(ixstb_descr_t));

        descr->pid = pid;
        descr->keys = get_key_struct();

        list_add_tail(&descr->list, &descr_list_.list);
    }

    if (even) {
        memcpy(descr->even, even, DESCR_KEY_LEN);
        set_even_control_word(descr->keys, even);
    }
    if (odd) {
        memcpy(descr->odd, odd, DESCR_KEY_LEN);
        set_odd_control_word(descr->keys, odd);
    }

test_key = get_key_struct();
set_control_words(test_key, even, odd);

print_dump("TEST KEY SET", even, 8);

    return pid;
}

int ixstb_descr_free_key(int handle)
{
    ixstb_descr_t   *descr = find_pid((uint16_t)handle);

    if (descr) {
        list_del(&descr->list);
        free_key_struct(descr->keys);
        free(descr);
    }

    return 0;
}

static unsigned char *cluster_[30 + 1];

int ixstb_descr_packets(int handle, uint8_t *data, ssize_t data_len)
{
#if 0
    ixstb_descr_t   *descr = find_pid((uint16_t)handle);

    if (descr) {
        unsigned char    *cluster[3];

        cluster[0] = data;
        cluster[1] = data + 188;
        cluster[2] = NULL;
        // print_dump("ORG", data, 188);
        print_debug("%d\n", decrypt_packets(descr->keys, cluster));
        // print_dump("DEC", data, 188);
    }
#endif
    // unsigned char    *cluster[3];

    // cluster[0] = data;
    // cluster[1] = data + 188;
    // cluster[2] = NULL;

    int     n, cluster;

    cluster = data_len / 188;

    for (n = 0; n <= cluster; n++) {
        cluster_[n] = data + n * 188;
    }
    cluster_[n] = NULL;

    decrypt_packets(test_key, cluster_);
    return 0;
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
