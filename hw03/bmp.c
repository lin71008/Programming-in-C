#include "bmp.h"

int32_t check_bmp_header(sBmpHeader *pHeader)
{
    if (pHeader == NULL) return 0;

    // check $pHeader->bm = "BM"
    if (pHeader->bm[0] != 'B' || pHeader->bm[1] != 'M') return 0;

    // check $pHeader->planes = 1
    if (pHeader->planes != 1) return 0;

    // success
    return 1;
}

int32_t load_bmp_header(FILE *fp, sBmpHeader *pHeader)
{
    if (fp == NULL || pHeader == NULL) return 0;
    fseek(fp, 0, SEEK_SET);

    // read sBmpHeader
    if (fread(pHeader, sizeof(sBmpHeader), 1, fp) != 1) return 0;

    // check header format
    return check_bmp_header(pHeader);
}

void print_bmp_header(const sBmpHeader *pHeader)
{
    printf("ID: %c%c\n", pHeader->bm[0], pHeader->bm[1]);
    printf("Size: %u\n", pHeader->flie_size);
    printf("Reserve: %u\n", pHeader->reserve);
    printf("Offset: %u\n", pHeader->offset);
    printf("Header Size: %u\n", pHeader->header_size);
    printf("Width: %u\n", pHeader->width);
    printf("Height: %u\n", pHeader->height);
    printf("Planes: %u\n", pHeader->planes);
    printf("Bits Per Pixel: %u\n", pHeader->bpp);
    printf("Compression: %u\n", pHeader->compression);
    printf("Bitmap Data Size: %u\n", pHeader->bitmap_size);
    printf("H-Resolution: %u\n", pHeader->hres);
    printf("V-Resolution: %u\n", pHeader->vres);
    printf("Used Colors: %u\n", pHeader->used);
    printf("Important Colors: %u\n", pHeader->important);
}

int32_t init_empty_BmpHandle(FILE *fp, sBmpHandle *pHandle, const sBmpHeader *pHeader)
{
    if (fp == NULL || pHandle == NULL || pHeader == NULL) return 0;

    pHandle->file = fp;
    pHandle->header = *pHeader;
    pHandle->offset = pHandle->header.offset;
    pHandle->bpp = pHandle->header.bpp;
    pHandle->color_set = pHandle->header.used;
    pHandle->height = abs(pHandle->header.height);
    pHandle->width = abs(pHandle->header.width);
    pHandle->padding = (32 - (pHandle->width * pHandle->bpp) % 32) % 32;

    fseek(pHandle->file, 0, SEEK_SET);
    fwrite(&(pHandle->header), sizeof(sBmpHeader), 1, pHandle->file);

    uint8_t EmptyUnit[4] = {255, 255, 255, 255};
    for (uint32_t h = 0; h < pHandle->height; ++h)
    {
        for (uint32_t w = 0; w < (pHandle->bpp * pHandle->width + 31) / 32; ++w)
        {
            fwrite(EmptyUnit, 1, 4, pHandle->file);
        }
    }
    return 1;
}

int32_t setup_BmpHandle(FILE *fp, sBmpHandle *pHandle)
{
    if (fp == NULL || pHandle == NULL) return 0;
    if (load_bmp_header(fp, &(pHandle->header)) != 1) return 0;

    pHandle->file = fp;
    pHandle->offset = pHandle->header.offset;
    pHandle->bpp = pHandle->header.bpp;
    pHandle->color_set = pHandle->header.used;
    pHandle->height = abs(pHandle->header.height);
    pHandle->width = abs(pHandle->header.width);
    pHandle->padding = (32 - (pHandle->width * pHandle->bpp) % 32) % 32;

    return 1;
}

void print_bmp_handle_info(const sBmpHandle *pHandle)
{
    printf("Offset: %u\n", pHandle->offset);
    printf("Bits per pixel: %u\n", pHandle->bpp);
    printf("Color set: (%u, %u, %u)\n", (pHandle->color_set >> 8) % 256, (pHandle->color_set >> 16) % 256, (pHandle->color_set >> 24) % 256);
    printf("Height (pixel): %u\n", pHandle->height);
    printf("Width (pixel): %u\n", pHandle->width);
    printf("Padding (bits): %u\n", pHandle->padding);
}

void get_pixel(uint8_t *pixel, sBmpHandle *pHandle, const uint32_t h, const uint32_t w)
{
    if (pixel == NULL || pHandle == NULL) return;
    if (h >= pHandle->height || w >= pHandle->width) return;

    uint32_t len = 1+(pHandle->bpp-1)/8;

    fseek(pHandle->file, pHandle->offset + h * ((pHandle->width * pHandle->bpp + pHandle->padding) / 8) + (w * pHandle->bpp / 8), SEEK_SET);
    fread(pixel, 1, len, pHandle->file);
}

void set_pixel(uint8_t *pixel, sBmpHandle *pHandle, const uint32_t h, const uint32_t w)
{
    if (pixel == NULL || pHandle == NULL) return;
    if (h >= pHandle->height || w >= pHandle->width) return;

    uint32_t len = 1+(pHandle->bpp-1)/8;

    fseek(pHandle->file, pHandle->offset + h * ((pHandle->width * pHandle->bpp + pHandle->padding) / 8) + (w * pHandle->bpp / 8), SEEK_SET);
    fwrite(pixel, 1, len, pHandle->file);
    // fread(pixel, 1, 1, pHandle->file);
    // fwrite(pixel, 1, 1, pHandle->file);
}
