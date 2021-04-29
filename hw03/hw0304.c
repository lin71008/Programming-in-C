#include <stdio.h>
#include <stdint.h>

#include "bmp.h"
#include "share.h"

int main()
{
    char InputFileName[256] = {0};
    char OutputFileName[256] = {0};
    char Buffer[256] = {0};
    int32_t Alpha = 0;

    printf("Please input a BMP file: ");
    fgets(InputFileName, 256, stdin);
    remove_newline(InputFileName);

    if (check_file_extension(InputFileName, "bmp") == 0)
    {
        printf("Error: File format not recognized.\n");
        return 0;
    }

    FILE *fp1 = NULL;
    if ((fp1 = fopen(InputFileName, "rb")) == NULL)
    {
        printf("Error: File could not be opened.\n");
        return 0;
    }

    sBmpHandle original;
    if (init_BmpHandle(fp1, &original) != 1)
    {
        printf("Error: File format not recognized.\n");
        fclose(fp1);
        return 0;
    }

    if (original.bpp != 24)
    {
        printf("Warning: File not 24-bit depth.\n");
        fclose(fp1);
        return 0;
    }

    printf("Please input the output BMP file name: ");
    fgets(OutputFileName, 256, stdin);
    remove_newline(OutputFileName);

    if (check_file_extension(OutputFileName, "bmp") == 0)
    {
        printf("Error: File format not recognized.\n");
        return 0;
    }

    FILE *fp2 = NULL;
    if ((fp2 = fopen(OutputFileName, "wb")) == NULL)
    {
        printf("Error: File could not be opened.\n");
        fclose(fp1);
        return 0;
    }

    printf("Alpha (0-31): ");
    fgets(Buffer, 256, stdin);
    while (sscanf(Buffer, "%d", &Alpha) != 1 || Alpha > 31 || Alpha < 0)
    {
        printf("Alpha (0-31): ");
        fgets(Buffer, 256, stdin);
    }

    sBmpHeader header = original.header;

    // expand header size
    // add alpha channel bitmask to header
    if (get_bmp_version(&header) < 4)
    {
        header.file_header.offset += (108 - header.dib_header.header_size);
        header.file_header.file_size += (108 - header.dib_header.header_size);

        header.dib_header.header_size = 108;

        header.dib_header.compression = 3;  // !important

        header.dib_header.red_channel_bitmask   = 0x00FF0000;
        header.dib_header.green_channel_bitmask = 0x0000FF00;
        header.dib_header.blue_channel_bitmask  = 0x000000FF;
        header.dib_header.alpha_channel_bitmask = 0x1F000000;

        header.dib_header.color_space_type[0] = 'W';
        header.dib_header.color_space_type[1] = 'i';
        header.dib_header.color_space_type[2] = 'n';
        header.dib_header.color_space_type[3] = ' ';

        uint32_t new_bitmap_size = header.dib_header.height * (1 + (header.dib_header.width * header.dib_header.bpp - 1) / 32) * 4;
        header.file_header.file_size += header.dib_header.bitmap_size - new_bitmap_size;
        header.dib_header.bitmap_size = new_bitmap_size;
    }

    sBmpHandle modified;
    init_empty_BmpHandle(fp2, &modified, &header);

    // print_bmp_handle(&original);
    // print_bmp_handle(&modified);

    uint8_t pixel[4] = {0};

    for (uint32_t h = 0; h < modified.height; ++h)
    {
        for (uint32_t w = 0; w < modified.width; ++w)
        {
            get_pixel(pixel, &original, h, w);
            pixel[3] = Alpha;
            set_pixel(pixel, &modified, h, w);
        }
    }

    fclose(fp2);
    fclose(fp1);

    return 0;
}
