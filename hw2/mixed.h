#ifndef __MIXED_H__
#define __MIXED_H__

#include <stdint.h>

typedef struct _sMixedNumber
{
    int32_t a;
    int32_t b;
    int32_t c;
} sMixedNumber;

extern int mixed_set(sMixedNumber *pNumber, int32_t a, int32_t b, int32_t c);
// return -1 if invalid; otherwise , return 0.

extern int mixed_print(const sMixedNumber number);
// in the form of (a,b,c)

extern void rational_add(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 + r2

extern void rational_sub(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 - r2

extern void rational_mul( sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 * r2

extern void rational_div( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 / r2

#endif
