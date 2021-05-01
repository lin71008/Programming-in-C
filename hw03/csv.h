#ifndef __CSV_H__
#define __CSV_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _sCsvFile sCsvFile;

#pragma pack(push)
#pragma pack(1)
struct _sCsvFile
{
    FILE *file;
};
#pragma pack(pop)

#endif
