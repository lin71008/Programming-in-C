#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <stdint.h>

typedef struct _sBigNum
{
    uint32_t sign: 1;  // 0: positive, 1: negative
    uint32_t size: 31;  // a_0, a_1, ... a_size, range: 0 ~ 2^28 (theory)
    uint64_t *data;  // a_i = c_(18*i)*10^(18*i) + ... c_(18*i+17)*10**(18*i+17)
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
