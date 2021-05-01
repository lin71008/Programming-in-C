#include "bmp.h"

int32_t get_bmp_version(const sBmpHeader *pHeader)
{
    if (pHeader->dib_header.header_size == 40) return 1;
    else if (pHeader->dib_header.header_size == 52) return 2;
    else if (pHeader->dib_header.header_size == 56) return 3;
    else if (pHeader->dib_header.header_size == 108) return 4;
    else if (pHeader->dib_header.header_size == 124) return 5;
    else return 0;
}

int32_t check_bmp_header(const sBmpHeader *pHeader)
{
    if (pHeader == NULL) return 0;

    // check $pHeader->bm = "BM"
    if (pHeader->file_header.bm[0] != 'B' || pHeader->file_header.bm[1] != 'M') return 0;

    // check $pHeader->planes = 1
    if (pHeader->dib_header.planes != 1) return 0;

    // check $pHeader->color_table
    if (pHeader->dib_header.used != 0 && pHeader->color_table == NULL) return 0;

    // success
    return 1;
}

int32_t load_bmp_header(FILE *fp, sBmpHeader *pHeader)
{
    if (fp == NULL || pHeader == NULL) return 0;

    fseek(fp, 0, SEEK_SET);
    // read $sBmpHeader->file_header
    if (fread(&pHeader->file_header, sizeof(sBmpFileHeader), 1, fp) != 1) return 0;

    uint32_t dib_header_size = 0;
    // get dib_header size
    if (fread(&dib_header_size, sizeof(uint32_t), 1, fp) != 1) return 0;
    fseek(fp, sizeof(sBmpFileHeader), SEEK_SET);

    // read $sBmpHeader->dib_header
    if (fread(&pHeader->dib_header, 1, dib_header_size, fp) != dib_header_size) return 0;

    // read $sBmpHeader->color_table
    // if (pHeader->dib_header.used > 0)
    // {
    //     pHeader->color_table = (uint8_t*) malloc(pHeader->dib_header.used * sizeof(uint8_t));
    //     if (fread(pHeader->color_table, 1, pHeader->dib_header.used, fp) != pHeader->dib_header.used)
    //     {
    //         free(pHeader->color_table);
    //         return 0;
    //     }
    // }
    // else
    // {
    //     pHeader->color_table = NULL;
    // }

    pHeader->color_table = NULL;
    pHeader->icc_color_profile = NULL;

    // check header format
    return check_bmp_header(pHeader);
}

void print_bmp_header(const sBmpHeader *pHeader)
{
    printf("\nBitmap v%d Header\n", get_bmp_version(pHeader));
    printf("ID:                  %10c%c\n", pHeader->file_header.bm[0], pHeader->file_header.bm[1]);
    printf("Size:                 %10u\n", pHeader->file_header.file_size);
    printf("Reserve:              %10u\n", pHeader->file_header.reserve);
    printf("Offset:               %10u\n", pHeader->file_header.offset);
    printf("Header Size:          %10u\n", pHeader->dib_header.header_size);
    printf("Width:                %10d\n", pHeader->dib_header.width);
    printf("Height:               %10d\n", pHeader->dib_header.height);
    printf("Planes:               %10u\n", pHeader->dib_header.planes);
    printf("Bits Per Pixel:       %10u\n", pHeader->dib_header.bpp);
    printf("Compression:          %10u\n", pHeader->dib_header.compression);
    printf("Bitmap Data Size:     %10u\n", pHeader->dib_header.bitmap_size);
    printf("H-Resolution:         %10u\n", pHeader->dib_header.hres);
    printf("V-Resolution:         %10u\n", pHeader->dib_header.vres);
    printf("Used Colors:          %10u\n", pHeader->dib_header.used);
    printf("Important Colors:     %10u\n", pHeader->dib_header.important);
    if (get_bmp_version(pHeader) >= 2)
    {
        printf("Red Channel Bitmask:    %08x\n", pHeader->dib_header.red_channel_bitmask);
        printf("Green Channel Bitmask:  %08x\n", pHeader->dib_header.green_channel_bitmask);
        printf("Blue Channel Bitmask:   %08x\n", pHeader->dib_header.blue_channel_bitmask);
    }
    if (get_bmp_version(pHeader) >= 3)
    {
        printf("Alpha Channel Bitmask:  %08x\n", pHeader->dib_header.alpha_channel_bitmask);
    }
    if (get_bmp_version(pHeader) >= 4)
    {
        printf("Color Space Type: %11c%c%c%c\n", pHeader->dib_header.color_space_type[0], pHeader->dib_header.color_space_type[1], pHeader->dib_header.color_space_type[2], pHeader->dib_header.color_space_type[3]);
    }
    if (pHeader->dib_header.used > 0)
    {
        printf("Color Table\n");
        for (uint32_t i = 0; i < pHeader->dib_header.used; ++i)
        {
            printf("  Colors %u: %08x\n", i, pHeader->color_table[i]);
        }
    }
}

int32_t init_BmpHandle(FILE *fp, sBmpHandle *pHandle)
{
    if (fp == NULL || pHandle == NULL) return 0;

    pHandle->file = fp;
    if (load_bmp_header(fp, &pHandle->header) != 1) return 0;

    pHandle->version = get_bmp_version(&pHandle->header);
    pHandle->offset = pHandle->header.file_header.offset;
    pHandle->bpp = pHandle->header.dib_header.bpp;
    pHandle->height = abs(pHandle->header.dib_header.height);
    pHandle->width = abs(pHandle->header.dib_header.width);
    pHandle->line = (1 + (pHandle->width * pHandle->bpp - 1) / 32) * 4;
    pHandle->get_pixel_offset = 0;
    pHandle->get_pixel_len = 0;

    return 1;
}
int32_t init_empty_BmpHandle(FILE *fp, sBmpHandle *pHandle, const sBmpHeader *pHeader)
{
    if (fp == NULL || pHandle == NULL || pHeader == NULL) return 0;

    pHandle->file = fp;
    if (check_bmp_header(pHeader) != 1) return 0;
    pHandle->header = *pHeader;

    pHandle->version = get_bmp_version(pHeader);
    pHandle->offset = pHeader->file_header.offset;
    pHandle->bpp = pHeader->dib_header.bpp;
    pHandle->height = abs(pHeader->dib_header.height);
    pHandle->width = abs(pHeader->dib_header.width);
    pHandle->line = (1 + (pHandle->width * pHandle->bpp - 1) / 32) * 4;
    pHandle->get_pixel_offset = 0;
    pHandle->get_pixel_len = 0;

    fseek(fp, 0, SEEK_SET);
    fwrite(&pHandle->header.file_header, sizeof(sBmpFileHeader), 1, fp);
    fwrite(&pHandle->header.dib_header, 1, pHeader->dib_header.header_size, fp);

    if (pHandle->header.dib_header.used > 0 && pHandle->header.color_table != NULL)
    {
        fwrite(&pHandle->header.color_table, 1, pHandle->header.dib_header.used, fp);
    }

    uint8_t EmptyUnit[4] = {0};

    // optional
    uint32_t gap1 = pHandle->offset - ftell(fp);
    if (gap1 > 0)
    {
        for (uint32_t i = 0; i < gap1; ++i)
        {
            fwrite(&EmptyUnit, 1, 1, fp);
        }
    }

    for (uint32_t h = 0; h < pHandle->height; ++h)
    {
        for (uint32_t w = 0; w < (pHandle->bpp * pHandle->width + 31) / 32; ++w)
        {
            fwrite(EmptyUnit, 1, 4, pHandle->file);
        }
    }

    return 1;
}

void print_bmp_handle(const sBmpHandle *pHandle)
{
    print_bmp_header(&pHandle->header);
}

int32_t get_pixel(uint8_t *pixel, sBmpHandle *pHandle, const uint32_t h, const uint32_t w)
{
    if (pixel == NULL || pHandle == NULL) return 0;
    if (h >= pHandle->height || w >= pHandle->width) return 0;

    if (pHandle->bpp % 8 == 0)
    {
        uint32_t read_offset = pHandle->offset + h * pHandle->line + (w * pHandle->bpp / 8);
        uint32_t len = pHandle->bpp / 8;

        if (pHandle->get_pixel_offset <= read_offset && read_offset + len <= pHandle->get_pixel_offset + pHandle->get_pixel_len)
        {
            // printf("%d: Copy (%d, %d) from Buffer[%d, %d]\n", pHandle->get_pixel_buffer + read_offset - pHandle->get_pixel_offset, read_offset, len, pHandle->get_pixel_offset, pHandle->get_pixel_len);
            memcpy(pixel, pHandle->get_pixel_buffer + read_offset - pHandle->get_pixel_offset, len);
        }
        else
        {
            pHandle->get_pixel_offset = read_offset;
            fseek(pHandle->file, read_offset, SEEK_SET);
            pHandle->get_pixel_len = fread(pHandle->get_pixel_buffer, 1, 1000, pHandle->file);
            // printf("%d: Copy (%d, %d) from Buffer[%d, %d]\n", pHandle->get_pixel_buffer + read_offset - pHandle->get_pixel_offset, read_offset, len, pHandle->get_pixel_offset, pHandle->get_pixel_len);
            memcpy(pixel, pHandle->get_pixel_buffer + read_offset - pHandle->get_pixel_offset, len);
        }
        return 1;
    }
    return 0;
}

int32_t set_pixel(uint8_t *pixel, sBmpHandle *pHandle, const uint32_t h, const uint32_t w)
{
    if (pixel == NULL || pHandle == NULL) return 0;
    if (h >= pHandle->height || w >= pHandle->width) return 0;

    if (pHandle->bpp % 8 == 0)
    {
        uint32_t read_offset = pHandle->offset + h * pHandle->line + (w * pHandle->bpp / 8);
        uint32_t len = pHandle->bpp / 8;

        fseek(pHandle->file, read_offset, SEEK_SET);
        fwrite(pixel, 1, len, pHandle->file);
        return 1;
    }
    return 0;
}
