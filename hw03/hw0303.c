#include <stdio.h>
#include <stdint.h>

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

    sBmpHandle modified;
    init_empty_BmpHandle(fp2, &modified, &header);

    print_bmp_handle(&original);
    print_bmp_handle(&modified);

    uint8_t pixel[4] = {0};

    for (uint32_t h = 0; h < modified.height; ++h)
    {
        for (uint32_t w = 0; w < modified.width; ++w)
        {
            get_pixel(pixel, &original, h, w);
            set_pixel(pixel, &modified, h, w);
        }
    }

    fclose(fp2);
    fclose(fp1);

    return 0;
}
