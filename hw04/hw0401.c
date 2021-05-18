#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "share.h"

const struct option long_options[] =
{
     { "prefix", 1, NULL, 'p' },
     { "size"  , 1, NULL, 'l' },
     { "help"  , 0, NULL, 'h' },
     { 0       , 0, 0   ,  0  },
};

// FLAG: xxxx xxxx xxxx xxxx
//       x                   -> set help
//        x                  -> set size
//         x                 -> set mode s
//          x                -> set mode r
//            x              -> set prefix (only effective at mode s)
//                 xxxx xxxx -> error flag
uint16_t FLAG = 0x0000;

void usage()
{
    fprintf(stderr, "Split:\n");
    fprintf(stderr, "   ./hw0401 -s target [--size <num>]\n");
    fprintf(stderr, "The default small file size is 1000 bytes.\n\n");
    fprintf(stderr, "Recover:\n");
    fprintf(stderr, "   ./hw0401 -r output file ...\n");
    fprintf(stderr, "The input small files may not be in order.\n");
}

int main(int argc, char **argv)
{
    int32_t size = 1000;
    char filename[256] = {0};
    char prefix[256] = {0};

    int32_t option = 0;
    while ((option = getopt_long(argc, argv, "r:s:", long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'h':
                FLAG = FLAG | 0x8000;
                break;
            case 'p':
                if (FLAG & 0x0800)
                {
                    FLAG = FLAG | 0x0008;
                }
                else
                {
                    strcpy(prefix, optarg);
                    FLAG = FLAG | 0x0800;
                }
                break;
            case 'r':
                if (FLAG & 0x4000)
                {
                    FLAG = FLAG | 0x0040;
                }
                else
                {
                    strcpy(filename, optarg);
                    FLAG = FLAG | 0x4000;
                }
                break;
            case 's':
                if (FLAG & 0x2000)
                {
                    FLAG = FLAG | 0x0020;
                }
                else
                {
                    strcpy(filename, optarg);
                    FLAG = FLAG | 0x2000;
                }
                break;
            case 'l':
                if (FLAG & 0x1000)
                {
                    FLAG = FLAG | 0x0010;
                }
                else
                {
                    size = atoi(optarg);
                    FLAG = FLAG | 0x1000;
                }
                break;
            case '?':
            default:
                FLAG = FLAG | 0x0080;
                break;
        }
    }

///////////////////////////////////////////////////////////////
// error check

    if (FLAG & 0x8000)
    {
        usage();
        return 0;
    }
    // Missing argument or set some argument more then one times
    if (!(!!(FLAG & 0x4000) ^ !!(FLAG & 0x2000)) || (FLAG & 0x0060))
    {
        fprintf(stderr, "Error: unknown mode option.\n");
        return 0;
    }
    if (FLAG & 0x0098)
    {
        fprintf(stderr, "Error: unknown argument.\n");
        return 0;
    }
    if (size <= 0)
    {
        fprintf(stderr, "Error: size option must bigger than zero.\n");
        return 0;
    }

///////////////////////////////////////////////////////////////
// Recover

    if (FLAG & 0x4000)
    {
        if ((argc - optind) == 0)
        {
            fprintf(stderr, "Error: file require.\n");
            return 0;
        }

        int8_t file[(argc - optind)];
        memset(file, 1, sizeof(file));

        for (int i = optind; i < argc; ++i)
        {
            int idx = atoi(file_extension(argv[i]));
            if (0 < idx && idx <= (argc - optind))
            {
                file[idx-1]--;
            }
        }
        for (int i = 0; i < (argc - optind); ++i)
        {
            if (file[i] != 0)
            {
                printf("Error: Missing some file.\n");
                return 0;
            }
        }

        FILE *fp = NULL;
        if ((fp = fopen(filename, "wb")) == NULL)
        {
            printf("Error: File could not be opened.\n");
            return 0;
        }

        for (int i = 1; i <= (argc - optind); ++i)
        {
            for (int j = optind; j < argc; ++j)
            {
                if (i == atoi(file_extension(argv[j])))
                {
                    FILE *fp2 = NULL;
                    if ((fp2 = fopen(argv[j], "rb")) == NULL)
                    {
                        printf("Error: File could not be opened.\n");
                        fclose(fp);
                        return 0;
                    }

                    uint8_t buffer[1000];
                    uint32_t buffer_size = 0;

                    while (!feof(fp2))
                    {
                        buffer_size = fread(buffer, 1, 1000, fp2);
                        fwrite(buffer, 1, buffer_size, fp);
                    }

                    fclose(fp2);
                    break;
                }
            }
        }
        fclose(fp);
    }

///////////////////////////////////////////////////////////////
// Split

    if (FLAG & 0x2000)
    {
        if (!(FLAG & 0x0800))
        {
            prefix[0] = '.';
            prefix[1] = '\0';
        }

        FILE *fp = NULL;
        if ((fp = fopen(filename, "rb")) == NULL)
        {
            printf("Error: File could not be opened.\n");
            return 0;
        }

        uint8_t buffer[1000];
        uint32_t buffer_size = 0;
        uint32_t file_remain_capacity = ((uint32_t) size);

        FILE *fp2 = NULL;
        char filename2[1000] = {0};
        uint32_t index = 1;

        sprintf(filename2, "%s/%s.%u", prefix, filename, index);
        if ((fp2 = fopen(filename2, "wb")) == NULL)
        {
            printf("Error: File could not be opened.\n");
            fclose(fp);
            return 0;
        }

        while (!feof(fp))
        {
            if (file_remain_capacity == 0)
            {
                fclose(fp2);

                index++;
                sprintf(filename2, "%s/%s.%u", prefix, filename, index);
                if ((fp2 = fopen(filename2, "wb")) == NULL)
                {
                    printf("Error: File could not be opened.\n");
                    fclose(fp);
                    return 0;
                }

                file_remain_capacity = ((uint32_t) size);
            }

            if (file_remain_capacity < 1000)
            {
                buffer_size = fread(buffer, 1, file_remain_capacity, fp);
            }
            else
            {
                buffer_size = fread(buffer, 1, 1000, fp);
            }

            fwrite(buffer, 1, buffer_size, fp2);
            file_remain_capacity -= buffer_size;
        }

        fclose(fp2);
        fclose(fp);
    }

///////////////////////////////////////////////////////////////
    return 0;
}

