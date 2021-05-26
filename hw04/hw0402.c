#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "share.h"

const struct option long_options[] =
{
     { "level" , 1, NULL, 'l' },
     { "input" , 1, NULL, 'i' },
     { "output", 1, NULL, 'o' },
     { "help"  , 0, NULL, 'h' },
     { 0       , 0, 0   ,  0  },
};

struct _sstatus
{
    int32_t multi_line;
    int32_t ascii_escape;
    enum _etype
    {
        macro,
        comment,
        multilinecomment,
        normal,
        string
    } type;
};

// FLAG: xxxx xxxx
//       x         -> set help
//        x        -> set output
//         x       -> set input
//          x      -> set level
//            xxxx -> error
uint8_t FLAG = 0x00;

void usage()
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    ./hw0402 -i <target> -o <output> -l <obfuscation level>\n");
    fprintf(stderr, "        Obfuscate the target file with a given level.\n");
    fprintf(stderr, "        The obfuscation level could be the following values:\n");
    fprintf(stderr, "            1: random padding.\n");
    fprintf(stderr, "            2: random variable name.\n");
    fprintf(stderr, "            3: random function name.\n");
    fprintf(stderr, "            4: unreadable integer constant.\n");
}

int main(int argc, char **argv)
{
    char input_filename[256] = {0};
    char output_filename[256] = {0};
    int32_t obfuscation_level = 0;

    int32_t option = 0;
    while ((option = getopt_long(argc, argv, "ho:i:l:", long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'h':
                FLAG = FLAG | 0x80;
                break;
            case 'o':
                if (FLAG & 0x40)
                {
                    FLAG = FLAG | 0x04;
                }
                else
                {
                    strcpy(output_filename, optarg);
                    FLAG = FLAG | 0x40;
                }
                break;
            case 'i':
                if (FLAG & 0x20)
                {
                    FLAG = FLAG | 0x02;
                }
                else
                {
                    strcpy(input_filename, optarg);
                    FLAG = FLAG | 0x20;
                }
                break;
            case 'l':
                if (FLAG & 0x10)
                {
                    FLAG = FLAG | 0x01;
                }
                else
                {
                    obfuscation_level = atoi(optarg);
                    FLAG = FLAG | 0x10;
                }
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
    // Missing argument or set some argument more then one times
    if ((FLAG & 0x0F) || (FLAG & 0x70) != 0x70)
    {
        if (FLAG & 0x08) fprintf(stderr, "Error: unknown argument\n");
        if ((FLAG & 0x70) != 0x70) fprintf(stderr, "Error: missing some argument\n");
        if (FLAG & 0x07) fprintf(stderr, "Error: set some argument more than one times\n");
        return 0;
    }

///////////////////////////////////////////////////////////////

    if (!(check_file_extension(input_filename, "c") || check_file_extension(input_filename, "h")))
    {
        fprintf(stderr, "Error: file format not recognized: %s\n", input_filename);
        return 0;
    }
    if (!(check_file_extension(output_filename, "c") || check_file_extension(output_filename, "h")))
    {
        fprintf(stderr, "Error: file format not recognized: %s\n", output_filename);
        return 0;
    }
    if (!(1 <= obfuscation_level && obfuscation_level <= 4))
    {
        fprintf(stderr, "Error: unknown obfuscation level.\n");
        return 0;
    }

///////////////////////////////////////////////////////////////

    FILE *fp1 = NULL;
    FILE *fp2 = NULL;

    if ((fp1 = fopen(input_filename, "r")) == NULL)
    {
       fprintf(stderr, "Error: File \"%s\" could not be opened.\n", input_filename);
        return 0;
    }

    if ((fp2 = fopen(output_filename, "w")) == NULL)
    {
        fclose(fp1);
       fprintf(stderr, "Error: File \"%s\" could not be opened.\n", output_filename);
        return 0;
    }

    char buffer[1000] = {0};
    int32_t buffer_size = 0;

    while (!feof(fp1))
    {
        buffer_size = fread(buffer, 1, 1000, fp1);
        fwrite(buffer, 1, buffer_size, fp2);
    }

    fclose(fp2);
    fclose(fp1);

    return 0;
}
