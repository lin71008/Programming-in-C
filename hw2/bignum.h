#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <stdint.h>

typedef struct _sBigNum
{
    uint32_t size;
    uint32_t max_size;
    int64_t *data;
} sBigNum;

extern void print(const sBigNum num);

extern int32_t set(sBigNum *pNum, char *str);

extern int32_t compare(const sBigNum num01, const sBigNum num02);

extern int32_t digits(const sBigNum num);

extern void add(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

extern void subtract(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

extern void multiply(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

extern void divide(sBigNum *pQuotient, sBigNum *pRemainder, const sBigNum num01, const sBigNum num02);

extern int32_t power(sBigNum *pResult, int32_t n, int32_t k);

extern int32_t combine(sBigNum *pResult, int32_t n, int32_t k);

#endif
