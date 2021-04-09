#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(push)
#pragma pack(1)
typedef struct _sBmpHeader
{
    char        bm[2];
    uint32_t    size;
    uint32_t    reserve;
    uint32_t    offset;
    uint32_t    header_size;
    int32_t    width;
    int32_t    height;
    uint16_t    planes;
    uint16_t    bpp;
    uint32_t    compression;
    uint32_t    bitmap_size;
    int32_t    hres;
    int32_t    vres;
    uint32_t    used;
    uint32_t    important;
} sBmpHeader;
#pragma pack(pop)

void print_bmp_header(sBmpHeader *pHeader)
{
    printf("ID: %c%c\n", pHeader -> bm[0], pHeader -> bm[1]);
    printf("Size: %u\n", pHeader -> size);
    printf("Reserve: %u\n", pHeader -> reserve);
    printf("Offset: %u\n", pHeader -> offset);
    printf("Header Size: %u\n", pHeader -> header_size);
    printf("Width: %u\n", pHeader -> width);
    printf("Height: %u\n", pHeader -> height);
    printf("Planes: %u\n", pHeader -> planes);
    printf("Bits Per Pixel: %u\n", pHeader -> bpp);
    printf("Compression: %u\n", pHeader -> compression);
    printf("Bitmap Data Size: %u\n", pHeader -> bitmap_size);
    printf("H-Resolution: %u\n", pHeader -> hres);
    printf("V-Resolution: %u\n", pHeader -> vres);
    printf("Used Colors: %u\n", pHeader -> used);
    printf("Important Colors: %u\n", pHeader -> important);
}

#pragma pack(push)
#pragma pack(1)
typedef struct _sPointer
{
    double x;
    double y;
} sPointer;
#pragma pack(pop)

sPointer Rotate_sPointer(const sPointer P, const double radian)
{
    sPointer Q = {0.0, 0.0};

    Q.x = cos(radian) * P.x - sin(radian) * P.y;
    Q.y = sin(radian) * P.x + cos(radian) * P.y;

    return Q;
}

static inline double min(double a, double b)
{
    return a < b ? a : b;
}

static inline double max(double a, double b)
{
    return a > b ? a : b;
}

int main(void)
{
    char iBuffer[1024] = {0};
    int angle = 0;

    printf("Please enter the file name: ");

    fgets(iBuffer, 1024, stdin);
    if (iBuffer[strlen(iBuffer)-1] == '\n') iBuffer[strlen(iBuffer)-1] = '\0';

    printf("Rotation angle (int , 0 -360): ");
    scanf("%d", &angle);

    FILE    *pFile = NULL;
    FILE    *pFile2 = NULL;

    if ((pFile = fopen(iBuffer, "rb")) == NULL)
    {
        printf("File could not be opened!\n");
        return 0;
    }

    if ((pFile2 = fopen("output.bmp", "wb")) == NULL)
    {
        printf("File could not be opened!\n");
        return 0;
    }

    sBmpHeader  header;
    fread(&header, sizeof(header), 1, pFile);

    int32_t original_heigh = header.height;
    int32_t original_width = header.width;
    int32_t original_padding = (4 - ((original_width * 3) % 4)) % 4;

    double radian = ((double) -angle) * M_PI / 180.0;

    int32_t modified_heigh = original_heigh * fabs(cos(radian)) + original_width * fabs(sin(radian));
    int32_t modified_width = original_heigh * fabs(sin(radian)) + original_width * fabs(cos(radian));
    int32_t modified_padding = (4 - ((modified_width * 3) % 4)) % 4;

    header.height = modified_heigh;
    header.width = modified_width;

    fwrite(&header, sizeof(header), 1, pFile2);

    uint8_t Pixel[3] = {255, 255, 255};

    double point_x[] = {  cos(radian) * 0.0            + sin(radian) * 0.0,
                          cos(radian) * original_width + sin(radian) * 0.0,
                          cos(radian) * 0.0            + sin(radian) * original_heigh,
                          cos(radian) * original_width + sin(radian) * original_heigh };

    double point_y[] = { -sin(radian) * 0.0            + cos(radian) * 0.0,
                         -sin(radian) * original_width + cos(radian) * 0.0,
                         -sin(radian) * 0.0            + cos(radian) * original_heigh,
                         -sin(radian) * original_width + cos(radian) * original_heigh };

    double modified_origin_x = min(min(point_x[0], point_x[1]), min(point_x[2], point_x[3]));
    double modified_origin_y = max(max(point_y[0], point_y[1]), max(point_y[2], point_y[3]));

    for (int32_t i = 0; i < modified_heigh; ++i)
    {
        for (int32_t j = 0; j < modified_width; ++j)
        {
            fwrite(Pixel, sizeof(uint8_t), 3, pFile2);
        }
        fwrite(Pixel, sizeof(uint8_t), modified_padding, pFile2);
    }

    fseek(pFile2, header.offset, SEEK_SET);
    for (int32_t i = 0; i < original_heigh; ++i)
    {
        for (int32_t j = 0; j < original_width; ++j)
        {
            fread(Pixel, sizeof(uint8_t), 3, pFile);

            double modified_x =  cos(radian) * j + sin(radian) * i;
            double modified_y = -sin(radian) * j + cos(radian) * i;

            double modified_real_x = modified_x - modified_origin_x;
            double modified_real_y = modified_heigh - (modified_origin_y - modified_y);

            fseek(pFile2, header.offset + ((3 * modified_width + modified_padding) * ((int) modified_real_y)) + (3 * ((int) modified_real_x)), SEEK_SET);
            fwrite(Pixel, sizeof(uint8_t), 3, pFile2);
        }
        fread(Pixel, sizeof(uint8_t), original_padding, pFile);
    }

    // for (int32_t i = 0; i < modified_heigh; ++i)
    // {
    //     for (int32_t j = 0; j < modified_width; ++j)
    //     {
    //         double original_x = cos(radian) * (modified_origin_x + j) - sin(radian) * (modified_origin_y - i);
    //         double original_y = sin(radian) * (modified_origin_x + j) + cos(radian) * (modified_origin_y - i);
    //         if (0 <= original_x && original_x < original_width && 0 <= original_y && original_y < original_heigh)
    //         {
    //             fseek(pFile, header.offset + (original_width * 3 + original_padding) * ((int) (original_heigh - original_y)) + 3 * ((int) original_x), SEEK_SET);
    //             fread(Pixel, sizeof(uint8_t), 3, pFile);
    //             fwrite(Pixel, sizeof(uint8_t), 3, pFile2);
    //         }
    //         else
    //         {
    //             Pixel[0] = 255, Pixel[1] = 255, Pixel[2] = 255;
    //             fwrite(Pixel, sizeof(uint8_t), 3, pFile2);
    //         }
    //     }
    //     Pixel[0] = 0, Pixel[1] = 0, Pixel[2] = 0;
    //     fwrite(Pixel, sizeof(uint8_t), modified_padding, pFile2);
    // }

    fclose(pFile);
    fclose(pFile2);

    return 0;
}
