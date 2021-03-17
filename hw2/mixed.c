#include <stdio.h>

#include "mixed.h"

int mixed_set(sMixedNumber *pNumber, int32_t a, int32_t b, int32_t c)
{
    if (pNumber == NULL || b < 0 || c < 0 || b >= c || (b != 0 && c == 0) || (b == 0 && c != 0))
    {
        return -1;
    }
    pNumber->a = a;
    pNumber->b = b;
    pNumber->c = c;
    return 0;
}

int mixed_print(const sMixedNumber number)
{
    printf("(%d, %d, %d)\n", number.a, number.b, number.c);
    return 0;
}

void rational_add(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if (pNumber == NULL) return;
    pNumber->a = r1.a + r2.a;
}

void rational_sub(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if (pNumber == NULL) return;
    pNumber->a = r1.a - r2.a;
}

void rational_mul( sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if (pNumber == NULL) return;
    pNumber->a = r1.a * r2.a;
}

void rational_div( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2)
{
    if (pNumber == NULL) return;
    pNumber->a = r1.a / r2.a;
}
