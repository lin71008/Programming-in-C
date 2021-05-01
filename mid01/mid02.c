#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(push)
#pragma pack(1)
typedef struct _sBmpHeader
{
    char     bm[2];
    uint32_t size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t bitmap_size;
    int32_t  hres;
    int32_t  vres;
    uint32_t used;
    uint32_t important;
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

void sBmpHeader_resize(sBmpHeader *header, const uint32_t h, const uint32_t w)
{
    int32_t padding = (32 - ((w * header->bpp) % 32)) % 32;

    header->height = h;
    header->width = w;
    header->bitmap_size = h * (w * header->bpp + padding) / 8;
    header->size = header->offset + header->bitmap_size;
}

void print_binary(uint32_t n)
{
    for (size_t i = 0; i < 32; ++i)
    {
        if (i && i % 4 == 0) printf(" ");
        if (n & (1 << (31 - i))) printf("1");
        else printf("0");
    }
    printf("\n");
}

#pragma pack(push)
#pragma pack(1)
typedef struct _sBitMap
{
    FILE *file;
    sBmpHeader *header;
    uint32_t offset;
    uint32_t bpp;
    uint32_t height;  // [row]
    uint32_t width;  // [pixle]
    uint32_t padding;  // [bits]
} sBitMap;
#pragma pack(pop)

void print_sBitMap_info(sBitMap* bm)
{
    printf("Offset: %u\n", bm->offset);
    printf("Bits per pixle: %u\n", bm->bpp);
    printf("Height (row): %u\n", bm->height);
    printf("Width (pixle): %u\n", bm->width);
    printf("Padding (bits): %u\n", bm->padding);
}

void sBitMap_init(sBitMap *bm, FILE *file, sBmpHeader *header)
{
    bm->file = file;
    bm->header = header;
    bm->offset = header->offset;
    bm->bpp = header->bpp;
    bm->height = abs(header->height);
    bm->width = abs(header->width);
    bm->padding = (32 - ((bm->width * bm->bpp) % 32)) % 32;
}

void sBitMap_init_empty(sBitMap *bm, FILE *file, sBmpHeader *header)
{
    bm->file = file;
    bm->header = header;
    bm->offset = header->offset;
    bm->bpp = header->bpp;
    bm->height = abs(header->height);
    bm->width = abs(header->width);
    bm->padding = (32 - ((bm->width * bm->bpp) % 32)) % 32;

    fseek(bm->file, 0, SEEK_SET);
    fwrite(bm->header, sizeof(sBmpHeader), 1, bm->file);

    uint8_t empty_bytes[1000];

    // default color setting
    memset(empty_bytes, 255, sizeof(uint8_t) * 1000);

    int64_t empty_bytes_amount = bm->header->bitmap_size;

    while (empty_bytes_amount > 1000)
    {
        fwrite(empty_bytes, sizeof(uint8_t), 1000, bm->file);
        empty_bytes_amount = empty_bytes_amount - 1000;
    }

    fwrite(empty_bytes, sizeof(uint8_t), empty_bytes_amount, bm->file);
}

// (R, G, B, A)
#define DEFAULT_COLOR {255, 255, 255, 255}

uint32_t sBitMap_get_pixel(sBitMap *bm, const uint32_t h, const uint32_t w)
{
    uint32_t Pixel = 0;
    uint8_t iPixel[4] = DEFAULT_COLOR;
    uint32_t Mask = (((1 << bm->bpp) - 1) << (32 - bm->bpp));

    Pixel = (iPixel[0] << 24) | (iPixel[1] << 16) | (iPixel[2] << 8) | (iPixel[3] << 0);

    if (h < bm->height && w < bm->width)
    {

        fseek(bm->file, bm->header->offset \
                        + (h * (bm->bpp * bm->width + bm->padding) / 8) \
                        + ((size_t) ((w * bm->bpp) / 8))\
                        , SEEK_SET);
        fread(iPixel, sizeof(uint8_t), ((size_t) ((bm->bpp + 7) / 8)), bm->file);

        Pixel = (iPixel[0] << 24) | (iPixel[1] << 16) | (iPixel[2] << 8) | (iPixel[3] << 0);
        Pixel = Pixel & (Mask >> ((bm->bpp * w) % 8));

        // printf("pF: %8ld, s = %u, pixle = %08x\n", ftell(bm->file), ((size_t) ((bm->bpp + 7) / 8)), Pixel);
    }
    return Pixel;
}

void sBitMap_set_pixel(sBitMap *bm, const uint32_t h, const uint32_t w, const uint32_t pixle)
{
    uint32_t Pixel = 0;
    uint8_t iPixel[4] = {0, 0, 0, 0};
    uint32_t Mask = (((1 << bm->bpp) - 1) << (32 - bm->bpp));

    if (h < bm->height && w < bm->width)
    {
        fseek(bm->file, bm->header->offset \
                        + (h * (bm->bpp * bm->width + bm->padding) / 8) \
                        + ((size_t) ((w * bm->bpp) / 8))\
                        , SEEK_SET);
        fread(iPixel, sizeof(uint8_t), ((size_t) ((bm->bpp + 7) / 8)), bm->file);

        Pixel = (iPixel[0] << 24) | (iPixel[1] << 16) | (iPixel[2] << 8) | (iPixel[3] << 0);
        Pixel = (Pixel & (0xFFFFFFFF ^ Mask)) | pixle;

        fseek(bm->file, bm->header->offset \
                        + (h * (bm->bpp * bm->width + bm->padding) / 8) \
                        + ((size_t) ((w * bm->bpp) / 8))\
                        , SEEK_SET);

        // printf("pF: %8ld, s = %u, pixle = %08x\n", ftell(bm->file), ((size_t) ((bm->bpp + 7) / 8)), Pixel);

        iPixel[0] = (Pixel & 0xFF000000) >> 24;
        iPixel[1] = (Pixel & 0x00FF0000) >> 16;
        iPixel[2] = (Pixel & 0x0000FF00) >>  8;
        iPixel[3] = (Pixel & 0x000000FF) >>  0;

        fwrite(iPixel, sizeof(uint8_t), ((size_t) ((bm->bpp + 7) / 8)), bm->file);
    }
}

#pragma pack(push)
#pragma pack(1)
typedef struct _sPointer
{
    double x;
    double y;
} sPointer;
#pragma pack(pop)

static inline sPointer sPointer_rotate(const sPointer P, const double radian)
{
    sPointer Q = {0.0, 0.0};

    Q.x =  cos(radian) * P.x + sin(radian) * P.y;
    Q.y = -sin(radian) * P.x + cos(radian) * P.y;

    return Q;
}

static inline double dmin(double x, double y)
{
    return x < y ? x : y;
}

static inline void sPointer_owo(sPointer *Result, const sPointer S[4])
{
    for (size_t i = 0; i < 4; ++i)
    {
        if (S[i].x < Result[0].x) Result[0].x = S[i].x;
        if (S[i].y < Result[0].y) Result[0].y = S[i].y;

        if (S[i].x > Result[1].x) Result[1].x = S[i].x;
        if (S[i].y > Result[1].y) Result[1].y = S[i].y;
    }
}

int main(void)
{
    char iBuffer[1024] = {0};
    double radian = 0.0;
    int angle = 0;

    printf("Please enter the file name: ");

    fgets(iBuffer, 1024, stdin);
    if (iBuffer[strlen(iBuffer)-1] == '\n') iBuffer[strlen(iBuffer)-1] = '\0';

    printf("Rotation angle (int , 0 ~ 360): ");
    scanf("%d", &angle);

    radian = ((double) angle) / 180.0 * M_PI;

    FILE *pFile = NULL;
    FILE *pFile2 = NULL;

    if ((pFile = fopen(iBuffer, "rb")) == NULL)
    {
        printf("File could not be opened!\n");
        return 0;
    }

    if ((pFile2 = fopen("output.bmp", "wb+")) == NULL)
    {
        printf("File could not be opened!\n");
        return 0;
    }

    sBmpHeader original_header;
    fread(&original_header, sizeof(sBmpHeader), 1, pFile);
    // print_bmp_header(&original_header);

    sBitMap original_bitmap;
    sBitMap_init(&original_bitmap, pFile, &original_header);
    // print_sBitMap_info(&original_bitmap);

    sPointer Vertex[4] = {sPointer_rotate(((sPointer) {original_bitmap.width, original_bitmap.height}), -radian), \
                          sPointer_rotate(((sPointer) {original_bitmap.width,                    0.0}), -radian), \
                          sPointer_rotate(((sPointer) {                  0.0, original_bitmap.height}), -radian), \
                          sPointer_rotate(((sPointer) {                  0.0,                    0.0}), -radian)};

    sPointer Range[2];
    sPointer_owo(Range, Vertex);
    // button left corner, top right corner

    // for (int i = 0; i < 4; ++i)
    // {
    //     printf("(%lf, %lf)\n", Vertex[i].x, Vertex[i].y);
    // }
    // printf("(%lf, %lf)\n", Range[0].x, Range[0].y);
    // printf("(%lf, %lf)\n", Range[1].x, Range[1].y);

    sBitMap modified_bitmap;
    sBmpHeader modified_header = original_header;

    sBmpHeader_resize(&modified_header, (Range[1].y - Range[0].y), (Range[1].x - Range[0].x));

    // initial blank bmp file
    sBitMap_init_empty(&modified_bitmap, pFile2, &modified_header);
    // print_sBitMap_info(&modified_bitmap);

    uint32_t Pixel = 0;

    for (uint32_t h = 0; h < modified_bitmap.height; ++h)
    {
        for (uint32_t w = 0; w < modified_bitmap.width; ++w)
        {
            sPointer P = {w + Range[0].x + 0.5, h + Range[0].y + 0.5};
            sPointer Q = sPointer_rotate(P, radian);

            Pixel = sBitMap_get_pixel(&original_bitmap, Q.y, Q.x);
            sBitMap_set_pixel(&modified_bitmap, h, w, Pixel);
        }
    }

    fclose(pFile);
    fclose(pFile2);

    return 0;
}
