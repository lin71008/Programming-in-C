#ifndef __CSV_H__
#define __CSV_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define BM_RGB32       { 0x0000FF00, 0x00FF0000, 0xFF000000, 0x00000000 }
#define BM_ARGB32      { 0x0000FF00, 0x00FF0000, 0xFF000000, 0x000000FF }
#define BM_RGB24       { 0x0000FF00, 0x00FF0000, 0xFF000000, 0x00000000 }
#define BM_A2R10G10B10 { 0x0000F03F, 0x00FC0F00, 0xFF030000, 0x000000C0 }
#define BM_A5R8G8B8    { 0x0000FF00, 0x00FF0000, 0xFF000000, 0x0000001F }

typedef struct _sBmpFileHeader sBmpFileHeader;
typedef struct _sBmpDIBHeader sBmpDIBHeader;
typedef struct _sBmpHeader sBmpHeader;
typedef struct _sBmpHandle sBmpHandle;

// 14 bytes
#pragma pack(push)
#pragma pack(1)
struct _sBmpFileHeader
{
    char        bm[2];  // "BM"
    uint32_t    file_size;  // [bytes]
    uint32_t    reserve;  // unused
    uint32_t    offset;  // begin point to pixel array
};
#pragma pack(pop)

// v1, v2, v3,  v4,  v5
// 40, 52, 56, 108, 124 bytes
#pragma pack(push)
#pragma pack(1)
struct _sBmpDIBHeader
{
    uint32_t    header_size;                 // size of DIP header
    int32_t     width;                       // [pixel]
    int32_t     height;                      // [pixel]
    uint16_t    planes;                      // default: 1, usage unknown
    uint16_t    bpp;                         // [bits/pixel]
    uint32_t    compression;                 // non-compression (default): 0, Bitfields (using bitmask): 1
    uint32_t    bitmap_size;                 // height * ceil(width * bpp, 32) / 4 [bytes]
    int32_t     hres;                        // [pixel/meter]
    int32_t     vres;                        // [pixel/meter]
    uint32_t    used;                        // default: 0
    uint32_t    important;                   // default: 0

    uint32_t    red_channel_bitmask;         // default: 0b 0000 0000 1111 1111 0000 0000 0000 0000
    uint32_t    green_channel_bitmask;       // default: 0b 0000 0000 0000 0000 1111 1111 0000 0000
    uint32_t    blue_channel_bitmask;        // default: 0b 0000 0000 0000 0000 0000 0000 1111 1111

    uint32_t    alpha_channel_bitmask;       // default: 0b 0001 1111 0000 0000 0000 0000 0000 0000

    char        color_space_type[4];         // "Win "
    uint32_t     color_space_endpoint[9];    // CIEXYZ, an 3 by 3 matrix
    uint16_t    red_channel_gamma[2];        // response curve, unsigned fixed 16.16 format
    uint16_t    green_channel_gamma[2];      // response curve, unsigned fixed 16.16 format
    uint16_t    blue_channel_gamma[2];       // response curve, unsigned fixed 16.16 format

    uint32_t    intent;
    uint32_t    icc_profile_data;
    uint32_t    icc_profile_size;            // [bytes]
    uint32_t    reserve;                     // default: 0,usage unknown
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct _sBmpHeader
{
    struct _sBmpFileHeader file_header;
    struct _sBmpDIBHeader dib_header;
    uint8_t* color_table;
    uint8_t* icc_color_profile;
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct _sBmpHandle
{
    FILE *file;
    struct _sBmpHeader header;
    uint32_t version;
    uint32_t offset;
    uint32_t bpp;
    uint32_t height;
    uint32_t width;

    uint32_t line;  // size of one row (include padding) [bytes]

    // reduce file io
    uint32_t get_pixel_offset;
    uint32_t get_pixel_len;
    uint8_t get_pixel_buffer[1000];
};
#pragma pack(pop)

// return 0 if format is unknown
// otherwise return positive number
extern int32_t get_bmp_version(const sBmpHeader *pHeader);

// return 1 if ...
//   - $pHeader->file_header.bm = "BM", and
//   - $pHeader->dib_header.planes = 1
// otherwise return 0
extern int32_t check_bmp_header(const sBmpHeader *pHeader);

// return 1 if ...
//   - fread($pHeader->file_header, $fp) success, and
//   - fread($pHeader->dib_header, $fp) success, and
//   - check_bmp_header($pHeader) = 1
// otherwise return 0
extern int32_t load_bmp_header(FILE *fp, sBmpHeader *pHeader);

// print header info
extern void print_bmp_header(const sBmpHeader *pHeader);

// update header
// extern void update_bmp_header(sBmpHeader *pHeader);

// return 1 if ...
//   - $fp and $pHandle exist, and
//   - $load_bmp_header() success
// otherwise return 0
extern int32_t init_BmpHandle(FILE *fp, sBmpHandle *pHandle);

// return 1 if ...
//   - $fp, $pHandle and $pHeader exist, and
//   - $pHeader pass $check_bmp_header()
// otherwise return 0
extern int32_t init_empty_BmpHandle(FILE *fp, sBmpHandle *pHandle, const sBmpHeader *pHeader);

// print handle info
extern void print_bmp_handle(const sBmpHandle *pHandle);

// return 1 if get/set success
// otherwise return 0
// Warning: unchecked that the length of $pixle is greater than $pHandle->bpp
extern int32_t get_pixel(uint8_t *pixel, sBmpHandle *pHandle, const uint32_t h, const uint32_t w);
extern int32_t set_pixel(uint8_t *pixel, sBmpHandle *pHandle, const uint32_t h, const uint32_t w);

#endif
