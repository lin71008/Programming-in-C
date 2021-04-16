#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <stdint.h>

typedef struct _sBigNum
{
    uint32_t sign: 1;  // 0: positive, 1: negative
    uint32_t size: 31;  // a_0, a_1, ... a_size, range: 0 ~ 2^31-1 (theory)
    uint8_t *data;  // a_i = c_(2*i)*10^(2*i) + c_(2*i+1)*10**(2*i+1)
} sBigNum;

/* example
123456 => {0, 3, [56, 34, 12]}
-12345 => {1, 3, [45, 23, 1]}
*/

extern void print(const sBigNum num);

extern int32_t set(sBigNum *pNum, char *str);
// return 0 if success

extern int32_t set_int(sBigNum *pNum, int32_t n);
// return 0 if success

extern int32_t compare(const sBigNum num01, const sBigNum num02);
// return 1 if num01 equal to num02

extern int32_t ubigger(const sBigNum num01, const sBigNum num02, const uint32_t bias);
// return 1 if num01 bigger than num02 (ignore sign bit)
// bias: add extra 00 at the end of num02
//// note: ubigger(a, a) retuen 1

extern int32_t digits(const sBigNum num);
// digits in base 10

extern void add(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

extern void subtract(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

extern void multiply(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

extern void divide(sBigNum *pQuotient, sBigNum *pRemainder, const sBigNum num01, const sBigNum num02);

extern int32_t power(sBigNum *pResult, int32_t n, int32_t k);
// return 0 if success

extern int32_t combine(sBigNum *pResult, int32_t n, int32_t k);
// return 0 if success

#endif
