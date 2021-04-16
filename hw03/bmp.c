#include "bmp.h"

void print_bmp_header(sBmpHeader *pHeader)
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
