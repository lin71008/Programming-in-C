#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <getopt.h>
#include <unistd.h>

#include "share.h"
#include "bst.h"

#pragma pack(push)
#pragma pack(1)
typedef struct _sFileItem
{
    char *name;
} sFileItem;
#pragma pack(pop)

sFileItem* sFileItem_create(char *name)
{
    sFileItem *newitem = (sFileItem*) malloc(sizeof(sFileItem));
    if (newitem == NULL) { return NULL; }
    newitem->name = strdup(name);
    return newitem;
}
void sFileItem_delete(void *target)
{
    if (target == NULL) { return; }
    if (((sFileItem*) target)->name != NULL) { free(((sFileItem*) target)->name); }
    free(((sFileItem*) target));
}

int cmpFileItem(const void *p1, const void *p2)
{
    if (p1 == NULL || p2 == NULL) { return 0; }
    if (strncmp(((sFileItem*)  p1)->name, ((sFileItem*)  p2)->name, strlen(((sFileItem*)  p2)->name)) == 0 && ((sFileItem*)  p2)->name[strlen(((sFileItem*)  p2)->name)-1] == '/') { return -1; }
    else { return 1; }
}

void sBST_print(sBSTNode **root, int tab)
{
    if (root == NULL) { return; }
    for (int i = 0; i < tab; ++i) { printf("    "); }
    char *c = strrchr(((sFileItem*) (*root)->data)->name, '/');
    if (c != NULL && *c != '\0' && *(c + 1) != '\0') { printf("+-- %s\n", (c + 1)); }
    else { printf("+-- %s\n", ((sFileItem*) (*root)->data)->name); }
    if ((*root)->left != NULL) { sBST_print(&((*root)->left), (tab + 1)); }
    if ((*root)->right != NULL) { sBST_print(&((*root)->right), tab); }
}

#pragma pack(push)
#pragma pack(1)
typedef struct _sZIPLocalHeader_Static
{
    char PK[4];
    uint16_t version;
    uint16_t purpose;
    char compress[2];
    uint16_t last_time;
    uint16_t last_date;
    int32_t crc32;
    uint32_t compress_size;
    uint32_t uncompress_size;
    uint16_t file_name_size;
    uint16_t extern_field_length;
} sZIPLocalHeader_Static;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct _sZIPLocalHeader
{
    struct _sZIPLocalHeader_Static info;
    char *file_name;
    char *extern_field;
} sZIPLocalHeader;
#pragma pack(pop)

#define getZIPLocalHeader(header, fp) \
    if ( fread((header), sizeof(sZIPLocalHeader_Static), 1, (fp)) == 1)\
    {\
        (header)->file_name = (char*) malloc(((header)->info.file_name_size + 1) * sizeof(char));\
        (header)->extern_field = (char*) malloc(((header)->info.extern_field_length + 1) * sizeof(char));\
        fread((header)->file_name, (header)->info.file_name_size * sizeof(char), 1, (fp));\
        (header)->file_name[(header)->info.file_name_size] = '\0';\
        fread((header)->extern_field, (header)->info.extern_field_length * sizeof(char), 1, (fp));\
        (header)->extern_field[(header)->info.extern_field_length] = '\0';\
    }

#define printZIPLocalHeader(header) \
    if ((header) != NULL)\
    {\
        printf("PK              : %c%c%c%c\n", (header)->info.PK[0], (header)->info.PK[1], (header)->info.PK[2], (header)->info.PK[3]);\
        printf("Version         : %u\n", (header)->info.version);\
        printf("Purpose         : %u\n", (header)->info.purpose);\
        printf("Compress Method : %d%d\n", (header)->info.compress[0], (header)->info.compress[1]);\
        printf("Last Time       : %u\n", (header)->info.last_time);\
        printf("Last Date       : %u\n", (header)->info.last_date);\
        printf("CRC32           : %d\n", (header)->info.crc32);\
        printf("Compress Size   : %u\n", (header)->info.compress_size);\
        printf("Uncompress Size : %u\n", (header)->info.uncompress_size);\
        printf("File Name       : %s\n", (header)->file_name);\
        printf("Extern Field    : %s\n", (header)->extern_field);\
    }


const struct option long_options[] =
{
     { "help"  , 0, NULL, 'h' },
     { 0       , 0, 0   ,  0  },
};

void usage()
{
    printf("Usage: hw0404 file...\n");
}

int main(int argc, char **argv)
{
    int8_t FLAG = 0x00;

    int32_t option = 0;
    while ((option = getopt_long(argc, argv, "h", long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'h':
                FLAG = FLAG | 0x80;
                break;
            case '?':
            default:
                FLAG = FLAG | 0x08;
                break;
        }
    }

///////////////////////////////////////////////////////////////

    // help
    if (FLAG & 0x80)
    {
        usage();
        return 0;
    }

    for (int i = optind; i < argc; ++i)
    {
        if (!check_file_extension(argv[i], "zip"))
        {
            printf("Error: file format not recognized: %s\n", argv[i]);
            return 0;
        }
    }

///////////////////////////////////////////////////////////////

    FILE *fp = NULL;

    for (int i = optind; i < argc; ++i)
    {
        if ((fp = fopen(argv[i], "rb")) == NULL)
        {
            printf("Error: File could not be opened.\n");
            return 0;
        }

        sBSTNode *root = NULL;
        sZIPLocalHeader header;

        printf("Zip File: %s\n", argv[i]);

        getZIPLocalHeader(&header, fp);

        while (header.info.file_name_size != 0)
        {
            sBST_add(&root, sFileItem_create(header.file_name), cmpFileItem);
            fseek(fp, header.info.compress_size, SEEK_CUR);
            getZIPLocalHeader(&header, fp);
        }

        sBST_print(&root, 0);

        sBSTNode_delete(&root, sFileItem_delete);
        fclose(fp);
    }

    return 0;
}
