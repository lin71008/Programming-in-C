#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mixed.h"

int32_t gcd(const int32_t a, const int32_t b)
{
    if (a == 0 && b == 0) return 1;
    else if (a == b || a == 0 || b == 0) return a | b;
    else if ((a == 1) | (b == 1)) return 1;
    else if (a > b) return gcd(a % b, b);
    else return gcd(a, b % a);
}

int mixed_set(sMixedNumber *pNumber, int32_t a, int32_t b, int32_t c)
{
    if (pNumber == NULL || !b != !c || (!!b && (c < 0 || (!!a && b < 0) || abs(b) >= c)) )
    {
        return -1;
    }
    pNumber->a = a;
    pNumber->b = b;
    pNumber->c = c;
    int32_t d = gcd(abs(b), c);
    if (d > 1)
    {
        pNumber->b = pNumber->b / d;
        pNumber->c = pNumber->c / d;
    }
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

    int32_t c1 = r1.c;
    if (c1 == 0) c1 = 1;
    int32_t c2 = r2.c;
    if (c2 == 0) c2 = 1;

    int32_t g = gcd(c1, c2);
    int32_t l = c1 * c2 / g;

    int32_t b1 = abs(r1.b) * c2 / g + abs(r1.a) * l;
    if (r1.a < 0 || r1.b < 0) b1 = -b1;

    int32_t b2 = abs(r2.b) * c1 / g + abs(r2.a) * l;
    if (r2.a < 0 || r2.b < 0) b2 = -b2;

    int32_t ret = b1 + b2;
    g = gcd(abs(ret), l);
    pNumber->a = ret / l;
    pNumber->b = (ret % l) / g;
    if (pNumber->a < 0) pNumber->b = -pNumber->b;
    pNumber->c = l / g;
    if (pNumber->b == 0) pNumber->c = 0;
}

void rational_sub(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if (pNumber == NULL) return;

    int32_t c1 = r1.c;
    if (c1 == 0) c1 = 1;
    int32_t c2 = r2.c;
    if (c2 == 0) c2 = 1;

    int32_t g = gcd(c1, c2);
    int32_t l = c1 * c2 / g;

    int32_t b1 = abs(r1.b) * c2 / g + abs(r1.a) * l;
    if (r1.a < 0 || r1.b < 0) b1 = -b1;

    int32_t b2 = abs(r2.b) * c1 / g + abs(r2.a) * l;
    if (r2.a < 0 || r2.b < 0) b2 = -b2;

    int32_t ret = b1 - b2;
    g = gcd(abs(ret), l);
    pNumber->a = ret / l;
    pNumber->b = (ret % l) / g;
    if (pNumber->a < 0) pNumber->b = -pNumber->b;
    pNumber->c = l / g;
    if (pNumber->b == 0) pNumber->c = 0;
}

void rational_mul( sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if (pNumber == NULL) return;

    int32_t c1 = r1.c;
    if (c1 == 0) c1 = 1;
    int32_t c2 = r2.c;
    if (c2 == 0) c2 = 1;

    int32_t b1 = abs(r1.b) + abs(r1.a) * c1;
    if (r1.a < 0 || r1.b < 0) b1 = -b1;

    int32_t b2 = abs(r2.b) + abs(r2.a) * c2;
    if (r2.a < 0 || r2.b < 0) b2 = -b2;

    int32_t b = b1 * b2;
    int32_t c = c1 * c2;

    int32_t g = gcd(abs(b), c);
    if (g > 1)
    {
        b = b / g;
        c = c / g;
    }
    pNumber->a = b / c;
    pNumber->b = (b % c);
    if (pNumber->a < 0) pNumber->b = -pNumber->b;
    pNumber->c = c;
    if (pNumber->b == 0) pNumber->c = 0;
}

void rational_div( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2)
{
   if (pNumber == NULL) return;

    int32_t c1 = r1.c;
    if (c1 == 0) c1 = 1;
    int32_t c2 = r2.c;
    if (c2 == 0) c2 = 1;

    int32_t b1 = abs(r1.b) + abs(r1.a) * c1;
    if (r1.a < 0 || r1.b < 0) b1 = -b1;

    int32_t b2 = abs(r2.b) + abs(r2.a) * c2;
    if (r2.a < 0 || r2.b < 0) b2 = -b2;

    int32_t b = b1 * c2;
    int32_t c = c1 * b2;
    if (c == 0) return;

    int32_t g = gcd(abs(b), c);
    if (g > 1)
    {
        b = b / g;
        c = c / g;
    }
    pNumber->a = b / c;
    pNumber->b = (b % c);
    if (pNumber->a < 0) pNumber->b = -pNumber->b;
    pNumber->c = c;
    if (pNumber->b == 0) pNumber->c = 0;
}
