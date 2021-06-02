#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

static int flag_enc = 0;
static int flag_dec = 0;
static int flag_output = 0;
static int flag_help = 0;
static int flag_rule = 0;
static int flag_error = 0;

static char encode_char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int padding = 1;
static int separators = 1;
static int length = 64;
static int8_t decode_char_set[256] = {0};

static const struct option long_options[] =
{
    { "enc",    1, NULL, 'e' },
    { "dec",    1, NULL, 'd' },
    { "output", 1, NULL, 'o' },
    { "help",   0, NULL, 'h' },
    { "rule",   1, NULL, 'r' },
    { 0       , 0,    0,  0  }
};

static void usage()
{
    printf("Usage:    ./hw0501 [options] <file>\n");
    printf("Options:\n");
    printf("  -e, --enc       Encode a file to a text file.\n");
    printf("  -d, --dec       Decode a text file to a file.\n");
    printf("  -o, --output    Output file name.\n");
    printf("  -r, --rule      Encoding rule.\n");
    printf("                  Supported: RFC-1421, RFC-2045, RFC-2152, RFC-3501,\n");
    printf("                             RFC-4648-4, RFC-4648-5, RFC-4880.\n");
    printf("                  *** DEFAULT USING RFC-1421 ***\n");
}

static inline int set_encoding(char *encode_rule)
{
    if (!encode_rule) return 1;
    if (!strncmp(encode_rule, "RFC-1421", 32))
    {
        encode_char_set[62] = '+';
        encode_char_set[63] = '/';
        padding = 1;
        separators = 1;
        length = 64;
    }
    else if (!strncmp(encode_rule, "RFC-2045", 32))
    {
        encode_char_set[62] = '+';
        encode_char_set[63] = '/';
        padding = 0;
        separators = 1;
        length = 76;
    }
    else if (!strncmp(encode_rule, "RFC-2152", 32))
    {
        encode_char_set[62] = '+';
        encode_char_set[63] = '/';
        padding = 0;
        separators = 0;
        length = 0;
    }
    else if (!strncmp(encode_rule, "RFC-3501", 32))
    {
        encode_char_set[62] = '+';
        encode_char_set[63] = ',';
        padding = 0;
        separators = 0;
        length = 0;
    }
    else if (!strncmp(encode_rule, "RFC-4648-4", 32))
    {
        encode_char_set[62] = '+';
        encode_char_set[63] = '/';
        padding = 1;
        separators = 0;
        length = 0;
    }
    else if (!strncmp(encode_rule, "RFC-4648-5", 32))
    {
        encode_char_set[62] = '-';
        encode_char_set[63] = '_';
        padding = 1;
        separators = 0;
        length = 0;
    }
    else if (!strncmp(encode_rule, "RFC-4880", 32))
    {
        encode_char_set[62] = '+';
        encode_char_set[63] = '/';
        padding = 1;
        separators = 1;
        length = 76;
    }
    else
    {
        return 1;
    }
    memset(decode_char_set, -1, 255);
    for (int i = 0; i < 64; ++i)
    {
        decode_char_set[((int8_t) encode_char_set[i])] = i;
    }
    decode_char_set[((int8_t) '=')] = 64;
    return 0;
}

int main(int argc, char **argv)
{
    int32_t option = 0;
    char input_filename[256] = {0};
    char output_filename[256] = {0};
    char encode_rule[32] = {0};

    while ((option = getopt_long(argc, argv, "e:d:o:hr:", long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'h':
                flag_help = 1;
                break;
            case 'e':
                if (!flag_enc & !flag_dec)
                {
                    flag_enc = 1;
                    strncpy(input_filename, optarg, 255);
                }
                else
                {
                    flag_error = 1;
                }
                break;
            case 'd':
                if (!flag_enc & !flag_dec)
                {
                    flag_dec = 1;
                    strncpy(input_filename, optarg, 255);
                }
                else
                {
                    flag_error = 1;
                }
                break;
            case 'o':
                if (!flag_output)
                {
                    flag_output = 1;
                    strncpy(output_filename, optarg, 255);
                }
                else
                {
                    flag_error = 1;
                }
                break;
            case 'r':
                if (!flag_rule)
                {
                    flag_rule = 1;
                    strncpy(encode_rule, optarg, 31);
                }
                else
                {
                    flag_error = 1;
                }
                break;
            case '?':
            default:
                flag_error = 1;
                break;
        }
    }
    if (flag_help)
    {
        usage();
        return 0;
    }
    if (flag_error || !flag_output || !(flag_enc ^ flag_dec))
    {
        printf("Error: please run ./hw0501 --help to see the usage.\n");
        return 0;
    }
    if (!flag_rule)
    {
        strcpy(encode_rule, "RFC-1421");
        flag_rule = 1;
    }
    if (set_encoding(encode_rule))
    {
        printf("Error: unsupported encoding.\n");
        return 0;
    }

    FILE *fp1 = NULL;
    FILE *fp2 = NULL;

    if ((fp1 = fopen(input_filename, "rb")) == NULL)
    {
        printf("Error: cannot open the given file.\n");
        return 0;
    }
    if ((fp2 = fopen(output_filename, "wb")) == NULL)
    {
        printf("Error: cannot open the output file.\n");
        fclose(fp1);
        return 0;
    }

    int8_t read_buffer[1200] = {0};
    int8_t write_buffer[1600] = {0};
    int read_buffer_size = 0;
    int write_buffer_size = 0;

    if (flag_enc)
    {
        while (!feof(fp1))
        {
            int ri = 0;
            memset(read_buffer, 0, 1200);
            read_buffer_size = fread(read_buffer, 1, 1200, fp1);
            for (ri = 0; ri <= (read_buffer_size - 3); ri += 3)
            {
                write_buffer[write_buffer_size++] = encode_char_set[read_buffer[ri+0] >> 2];
                write_buffer[write_buffer_size++] = encode_char_set[(read_buffer[ri+0] & 0x03) << 4 | read_buffer[ri+1] >> 4];
                write_buffer[write_buffer_size++] = encode_char_set[(read_buffer[ri+1] & 0x0F) << 2 | read_buffer[ri+2] >> 6];
                write_buffer[write_buffer_size++] = encode_char_set[read_buffer[ri+2] & 0x3F];
            }
            int remain = read_buffer_size - ri;
            if (remain > 0) write_buffer[write_buffer_size++] = encode_char_set[read_buffer[ri+0] >> 2];
            if (remain > 0) write_buffer[write_buffer_size++] = encode_char_set[(read_buffer[ri+0] & 0x03) << 4 | read_buffer[ri+1] >> 4];
            if (remain > 1) write_buffer[write_buffer_size++] = encode_char_set[(read_buffer[ri+1] & 0x0F) << 2 | read_buffer[ri+2] >> 6];
            for (int i = 0; padding && i < (3 - remain); ++i)
            {
                write_buffer[write_buffer_size++] = '=';
            }
            if (separators)
            {
                int wi = 0;
                for (wi = 0; wi <= (write_buffer_size - length); wi += length)
                {
                    fwrite(write_buffer+wi, 1, length, fp2);
                    fprintf(fp2, "\n");
                }
                int wremain = write_buffer_size - wi;
                if (wremain) memcpy(write_buffer, write_buffer+wi, wremain);
                write_buffer_size = wremain;
                if (feof(fp1)) fwrite(write_buffer, 1, write_buffer_size, fp2);
            }
            else
            {
                fwrite(write_buffer, 1, write_buffer_size, fp2);
                write_buffer_size = 0;
            }
        }
    }

    if (flag_dec)
    {
        while (!feof(fp1))
        {
            int ri = 0;
            write_buffer_size = 0;
            read_buffer_size += fread(read_buffer+read_buffer_size, 1, 1200, fp1);
            for (ri = 0; ri <= (read_buffer_size - 4); ri += 4)
            {
                while (read_buffer[ri] == '\n' && ri <= (read_buffer_size - 4))
                {
                    ri++;
                }

                int8_t a0 = decode_char_set[read_buffer[ri+0]];
                int8_t a1 = decode_char_set[read_buffer[ri+1]];
                int8_t a2 = decode_char_set[read_buffer[ri+2]];
                int8_t a3 = decode_char_set[read_buffer[ri+3]];

                if (a0 == -1 || a1 == -1 || a2 == -1 || a3 == -1 || a0 == 64 || a1 == 64 || ((a2 == 64 || a3 == 64) && !feof(fp1)))
                {
                    printf("Error: wrong encoding or given file contain invalid character.\n");
                    fclose(fp2);
                    fclose(fp1);
                    return 0;
                }

                write_buffer[write_buffer_size++] = (a0 << 2) | ((a1 & 0x3F) >> 4);
                if (a2 != 64) write_buffer[write_buffer_size++] = (a1 << 4) | ((a2 & 0x3F) >> 2);
                if (a3 != 64) write_buffer[write_buffer_size++] = (a2 << 6) | ((a3 & 0x3F));
            }
            int remain = read_buffer_size - ri;
            if (remain) memcpy(read_buffer, read_buffer+ri, remain);
            read_buffer_size = remain;
            if (feof(fp1) && remain)  // no padding
            {
                int8_t a0 = decode_char_set[read_buffer[0]];
                int8_t a1 = decode_char_set[read_buffer[1]];
                int8_t a2 = decode_char_set[read_buffer[2]];

                if (remain == 2 && a0 != -1 && a0 != 64 && a1 != -1 && a1 != 64)
                {
                    write_buffer[write_buffer_size++] = (a0 << 2) | (a1 >> 4);
                }
                else if (remain == 3 && a0 != -1 && a0 != 64 && a1 != -1 && a1 != 64 && a2 != -1 && a2 != 64)
                {
                    write_buffer[write_buffer_size++] = (a0 << 2) | (a1 >> 4);
                    write_buffer[write_buffer_size++] = (a1 << 4) | (a2 >> 2);
                }
                else
                {
                    printf("Error: file format unrecognized.\n");
                    fclose(fp2);
                    fclose(fp1);
                    return 0;
                }
            }
            fwrite(write_buffer, 1, write_buffer_size, fp2);
        }
    }

    fclose(fp2);
    fclose(fp1);
    return 0;
}

