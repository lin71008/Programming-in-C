#include <stdio.h>
#include <stdint.h>

int main()
{
    double X;
    printf("Please enter a floating-point number (double precision): ");
    scanf("%lf", &X);

    const uint64_t * const pX = (const uint64_t * const)&X;

    uint64_t bit = 1;
    bit = bit << 63;

    printf("Sign: ");
    if (bit & *pX) printf("1");
    else printf("0");
    bit = bit >> 1;
    printf("\n");

    printf("Exponent: ");
    for (int i = 0; i < 11; ++i)
    {
        if (bit & *pX) printf("1");
        else printf("0");
        bit = bit >> 1;
    }
    printf("\n");

    printf("Fraction: ");
    for (int i = 0; i < 52; ++i)
    {
        if (bit & *pX) printf("1");
        else printf("0");
        bit = bit >> 1;
    }
    printf("\n");

    uint16_t e = 0;
    bit = (uint64_t) 1 << 63;
    printf("%lf = (-1)^", X);
    if (bit & *pX) printf("1 * (1");
    else printf("0 * (1");
    bit = bit >> 1;
    for (int i = 0; i < 11; ++i)
    {
        e = e << 1;
        if (bit & *pX) e = e | 1;
        bit = bit >> 1;
    }
    for (int i = 0; i < 52; ++i)
    {
        if (bit & *pX) printf(" + 2^-%d", i+1);
        bit = bit >> 1;
    }
    printf(") * 2^(1023-%d)\n", e);


    return 0;
}
