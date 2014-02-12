/* ******************************************************************************************************************
 * spidercast.dev@gmail.com
 * ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include "ixdbm_parser.h"

/* ****************************************************************************************************************** */

static void help()
{
    printf("Usage: dbmaker [OPTION]\n");
    printf("       -h            help\n");
    printf("       -f [FILE]     source XML file (enigma2 format)\n");
}

int main(int argc, char *argv[])
{
    int     fd;
    char    xml_fname[128];
    char    dbm_fname[128];
    int     n;

    memset(xml_fname, 0, sizeof(xml_fname));
    strcpy(dbm_fname, "default_v2.db");

    while ((n = getopt(argc, argv, "hf:")) != -1) {
        switch (n) {
        case 'f':
            strcpy(xml_fname, optarg);
            break;
        case 'h':
        default:
            help();
            exit(0);
        }
    }

    if (strlen(xml_fname) == 0) {
        help();
        exit(0);
    }

    if ((fd = open(xml_fname, O_RDONLY)) < 0) {
        fprintf(stderr, "file open error: %s\n", xml_fname);
        exit(0);
    }

    close(fd);

    if (access(dbm_fname, F_OK) == 0)
        unlink(dbm_fname);

    return ixdbm_parser(xml_fname, dbm_fname);
}

/* ****************************************************************************************************************** */

/* end of file ****************************************************************************************************** */
