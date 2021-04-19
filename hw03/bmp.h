#ifndef __CSV_H__
#define __CSV_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define DEFAULT_COLOR_24 {255, 255, 255}
#define DEFAULT_COLOR_32 {255, 255, 255, 255}

typedef struct _sBmpHeader sBmpHeader;
typedef struct _sBmpHandle sBmpHandle;

// return 1 if ...
//   - $pHeader->bm = "BM", and
//   - $pHeader->planes = 1
// otherwwise return 0
extern int32_t check_bmp_header(sBmpHeader *pHeader);

// return 1 if ...
//   - fread($pHeader, $fp) success, and
//   - check_bmp_header($pHeader) = 1
// otherwwise return 0
extern int32_t load_bmp_header(FILE *fp, sBmpHeader *pHeader);
extern void print_bmp_header(const sBmpHeader *pHeader);


// return 1 if ... success
//   - $fp, $pHandle and $pHeader exist, and
//   - $pHeader pass $check_bmp_header()
// otherwwise return 0
extern int32_t init_empty_BmpHandle(FILE *fp, sBmpHandle *pHandle, const sBmpHeader *pHeader);

// return 1 if ... success
//   - $fp and $pHandle exist, and
//   - $load_bmp_header() success
// otherwwise return 0
extern int32_t setup_BmpHandle(FILE *fp, sBmpHandle *pHandle);
extern void print_bmp_handle_info(const sBmpHandle *pHandle);

// Warning: unchecked that the length of $pixle is greater than $pHandle->bpp
extern void get_pixel(uint8_t *pixel, sBmpHandle *pHandle, const uint32_t h, const uint32_t w);
extern void set_pixel(uint8_t *pixel, sBmpHandle *pHandle, const uint32_t h, const uint32_t w);

// update $flie_size, $bitmap_size
// extern void update_header(sBmpHandle *pHandle);

#pragma pack(push)
#pragma pack(1)
struct _sBmpHeader
{
    char        bm[2];  // "BM"
    uint32_t    flie_size;  // [bytes]
    uint32_t    reserve;  // unused
    uint32_t    offset;  // begin point to bmp body
    // DIP header
    uint32_t    header_size;  // size of DIP header
    int32_t     width;  // [pixel]
    int32_t     height;  // [pixel]
    uint16_t    planes;  // 1
    uint16_t    bpp;  // [bits/pixel]
    uint32_t    compression;
    uint32_t    bitmap_size;
    int32_t     hres;  // [pixel/meter]
    int32_t     vres;  // [pixel/meter]
    uint32_t    used;  // 0: default; Byte(B, G, R, _)
    uint32_t    important;  // default: 0
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct _sBmpHandle
{
    FILE *file;
    struct _sBmpHeader header;
    uint32_t offset;
    uint32_t bpp;
    uint32_t color_set;
    uint32_t height;
    uint32_t width;
    uint32_t padding;  // 0 ~ 31 [bits]
};
#pragma pack(pop)

#endif
