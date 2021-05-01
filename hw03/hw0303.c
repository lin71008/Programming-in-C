#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "bmp.h"
#include "share.h"

int main()
{
    char InputFileName[256] = {0};
    char OutputFileName[256] = {0};
    char Buffer[256] = {0};
    int32_t Angle = 0;

    printf("Please input a BMP file: ");
    fgets(InputFileName, 256, stdin);
    remove_newline(InputFileName);

    if (check_file_extension(InputFileName, "bmp") == 0)
    {
        printf("Warning: File format not recognized.\n");
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

    printf("Please input the output BMP file name: ");
    fgets(OutputFileName, 256, stdin);
    remove_newline(OutputFileName);

    if (check_file_extension(OutputFileName, "bmp") == 0)
    {
        printf("Warning: File format not recognized.\n");
        return 0;
    }

    FILE *fp2 = NULL;
    if ((fp2 = fopen(OutputFileName, "wb")) == NULL)
    {
        printf("Error: File could not be opened.\n");
        fclose(fp1);
        return 0;
    }

    printf("Angle (0-90): ");
    fgets(Buffer, 256, stdin);
    while (sscanf(Buffer, "%d", &Angle) != 1 || Angle < 0 || Angle > 90)
    {
        printf("Angle (0-90): ");
        fgets(Buffer, 256, stdin);
    }

    sBmpHeader header = original.header;

    const uint32_t width = abs(header.dib_header.width);
    const uint32_t height = abs(header.dib_header.height);

    header.dib_header.width = width + height * cos(Angle * M_PI / 180.0);
    header.dib_header.height = height * sin(Angle * M_PI / 180.0);
    if (header.dib_header.height == 0) header.dib_header.height = 1;

    sBmpHandle modified;
    init_empty_BmpHandle(fp2, &modified, &header);

    // print_bmp_handle(&original);
    // print_bmp_handle(&modified);

    uint8_t pixel[4] = {0};
    uint8_t empty_pixel[4] = {255, 255, 255, 255};

    for (uint32_t h = 0; h < modified.height; ++h)
    {
        for (uint32_t w = 0; w < modified.width; ++w)
        {
            if (tan(Angle * M_PI / 180.0) != 0 && h / tan(Angle * M_PI / 180.0) < w && w < width + h / tan(Angle * M_PI / 180.0))
            {
                get_pixel(pixel, &original, h / sin(Angle * M_PI / 180.0), w - h / tan(Angle * M_PI / 180.0));
                set_pixel(pixel, &modified, h, w);
            }
            else
            {
                set_pixel(empty_pixel, &modified, h, w);
            }
        }
    }

    fclose(fp2);
    fclose(fp1);

    return 0;
}
