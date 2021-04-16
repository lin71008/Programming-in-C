#ifndef __CSV_H__
#define __CSV_H__

#include <stdio.h>
#include <stdint.h>

typedef struct _sBmpHeader sBmpHeader;

extern void print_bmp_header(sBmpHeader *pHeader);

#pragma pack(push)
#pragma pack(1)
struct _sBmpHeader
{
    char        bm[2];
    uint32_t    flie_size;
    uint32_t    reserve;
    uint32_t    offset;
    uint32_t    header_size;
    int32_t     width;
    int32_t     height;
    uint16_t    planes;
    uint16_t    bpp;
    uint32_t    compression;
    uint32_t    bitmap_size;
    int32_t     hres;
    int32_t     vres;
    uint32_t    used;
    uint32_t    important;
};
#pragma pack(pop)

#endif
