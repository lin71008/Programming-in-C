#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>

#include "bignum.h"

#define M 9
// each cell contain M digits

#define mPRId64 "9"PRId64

void print(const sBigNum num)
{
    if (num.data == NULL || num.size == 0) return;

    if (num.sign == 1) printf("-");

    printf("[[%"PRId64"]]", num.data[num.size-1]);
    for (uint32_t i = 1; i < num.size; ++i)
    {
        printf("[%0"mPRId64"]", num.data[num.size-1-i]);
    }

    printf("\n");
}

int32_t set(sBigNum *pNum, char *str)
{
    if (pNum== NULL || str == NULL) return -1;

    if (str[0] == '-')
    {
        pNum->sign = 1;
        str++;
    }
    else
    {
        pNum->sign = 0;
    }

    pNum->size = 0;
    uint32_t n = strlen(str);
    if (n & (1 << 31)) return -1; // string is too long

    pNum->data = (uint64_t*) malloc((1 + (n - 1) / M) * sizeof(uint64_t));
    if (pNum->data == NULL) return -1; // run out of memory

    pNum->size = (1 + (n - 1) / M);

    uint32_t buffer = 0;
    for (uint32_t i = 0; i < n; ++i)
    {
        if (!isdigit(str[i])) return -1;  // string contain invalid character

        buffer = buffer * 10 + (str[i] - '0');
        if ((n-i-1) % M == 0)
        {
            pNum->data[(n-i-1) / M] = buffer;
            buffer = 0;
        }
    }

    if (pNum->data[pNum->size-1] == 0) pNum->sign = 0;  // deal with -0 issue.

    return 0;
}

int32_t compare(const sBigNum num01, const sBigNum num02)
{
    if (num01.sign != num02.sign || num01.size != num02.size) return 0;
    for (uint32_t i = 0; i < num01.size; ++i)
    {
        if (num01.data[i] != num02.data[i]) return 0;
    }
    return 1;
}

static inline uint8_t uint64_digit(uint64_t n)
{
    uint8_t len = 0;
    while (n > 0)
    {
        len = len + 1;
        n = n / 10;
    }
    return len;
}

int32_t digits(const sBigNum num)
{
    if (num.data == NULL || num.size == 0) return -1;
    else if (num.data[num.size-1] == 0) return 1;
    else return uint64_digit(num.data[num.size-1]) + ((num.size-1) * M);
}

void add(sBigNum *pResult, const sBigNum num01, const sBigNum num02)
{

}

void subtract(sBigNum *pResult, const sBigNum num01, const sBigNum num02)
{

}

void multiply(sBigNum *pResult, const sBigNum num01, const sBigNum num02)
{

}

void divide(sBigNum *pQuotient, sBigNum *pRemainder, const sBigNum num01, const sBigNum num02)
{

}

int32_t power(sBigNum *pResult, int32_t n, int32_t k)
{
    return 0;
}

int32_t combine(sBigNum *pResult, int32_t n, int32_t k)
{
    return 0;
}
