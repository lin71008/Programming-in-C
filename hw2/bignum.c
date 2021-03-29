#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bignum.h"

void print(const sBigNum num)
{
    if (num.data == NULL || num.size == 0) return;

    if (num.sign == 1) printf("-");

    printf("%d", num.data[num.size-1]);
    for (uint32_t i = 1; i < num.size; ++i)
    {
        printf("%02d", num.data[num.size-1-i]);
    }

    printf("\n");
}

int32_t set(sBigNum *pNum, char *str)
{
    if (pNum == NULL || str == NULL) return -1;

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
    pNum->data = (uint8_t*) malloc((1 + (n - 1) / 2) * sizeof(uint8_t));
    if (pNum->data == NULL) return -1; // run out of memory
    pNum->size = (1 + (n - 1) / 2);


    uint32_t buffer = 0;
    for (uint32_t i = 0; i < n; ++i)
    {
        if (!isdigit(str[i])) return -1;  // string contain invalid character

        buffer = buffer * 10 + (str[i] - '0');
        if ((n-i-1) % 2 == 0)
        {
            pNum->data[(n-i-1) / 2] = buffer;
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

int32_t ubigger(const sBigNum num01, const sBigNum num02, const uint32_t bias)
{
    if (num01.size != (num02.size + bias)) return num01.size > (num02.size + bias);
    for (uint32_t i = 0; i < num02.size; ++i)
    {
        if (num01.data[num01.size-1-i] < num02.data[num02.size-1-i]) return 0;
    }
    return 1;
}

static inline uint8_t uint8_digit(uint8_t n)
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
    else return uint8_digit(num.data[num.size-1]) + ((num.size-1) * 2);
}

static inline uint32_t max(uint32_t a, uint32_t b)
{
    if (a > b) return a;
    else return b;
}

static void _add(sBigNum *pResult, const sBigNum num01, const sBigNum num02, const uint32_t bias)
{
    pResult->size = 0;
    pResult->data = (uint8_t*) malloc((max(num01.size, num02.size+bias)+1) * sizeof(uint8_t));
    if (pResult->data == NULL) return;  // run out of memory.
    pResult->size = max(num01.size, num02.size+bias) + 1;

    for (uint32_t i = 0; i < pResult->size; ++i)  // initial
    {
        pResult->data[i] = 0;
    }

    for (uint32_t i = 0; i < num01.size; ++i)  // set
    {
        pResult->data[i] = num01.data[i];
    }

    for (uint32_t i = 0; i < num02.size; ++i)  // add
    {
        pResult->data[i+bias] = pResult->data[i+bias] + num02.data[i];
    }

    for (uint32_t i = 0; i+1 < pResult->size; ++i)  // carry
    {
        pResult->data[i+1] = pResult->data[i+1] + pResult->data[i] / 100;
        pResult->data[i] = pResult->data[i] % 100;
    }

    if (pResult->data[pResult->size-1] == 0)  // remove unused
    {
        pResult->size = pResult->size - 1;
        pResult->data = (uint8_t*) realloc(pResult->data, pResult->size*sizeof(uint8_t));
    }
}

static void _sub(sBigNum *pResult, const sBigNum num01, const sBigNum num02, const uint32_t bias)  // note: |num01| >= |num02|
{
    pResult->size = 0;
    pResult->data = (uint8_t*) malloc(num01.size * sizeof(uint8_t));
    if (pResult->data == NULL) return;  // run out of memory.
    pResult->size = num01.size;

    for (uint32_t i = 0; i < pResult->size; ++i)  // initial
    {
        pResult->data[i] = 0;
    }

    for (uint32_t i = 0; i < num01.size; ++i)  // set
    {
        pResult->data[i] = num01.data[i];
    }

    uint8_t buffer = 0;
    for (uint32_t i = bias; i < num01.size; ++i)  // subtract
    {
        if (i-bias < num02.size)
        {
            if (num01.data[i] >= num02.data[i-bias] + buffer)
            {
                pResult->data[i] = pResult->data[i] - (num02.data[i-bias] + buffer);
                buffer = 0;
            }
            else
            {
                pResult->data[i] = pResult->data[i] + 100 - (num02.data[i-bias] + buffer);
                buffer = 1;
            }
        }
        else
        {
            if (num01.data[i] >= buffer)
            {
                pResult->data[i] = pResult->data[i] - buffer;
                buffer = 0;
                break;
            }
            else
            {
                pResult->data[i] = pResult->data[i] + 100 - buffer;
                buffer = 1;
            }
        }

    }

    uint32_t unused = 0;  // remove unused
    for (uint32_t i = 0; i+1 < pResult->size; ++i)
    {
        if (pResult->data[pResult->size-1-i] == 0) unused = unused + 1;
        else break;
    }

    pResult->size = pResult->size - unused;
    pResult->data = (uint8_t*) realloc(pResult->data, pResult->size*sizeof(uint8_t));

    if (pResult->data[pResult->size-1] == 0) pResult->sign = 0;  // deal with -0 issue.
}

void add(sBigNum *pResult, const sBigNum num01, const sBigNum num02)
{
    if (pResult == NULL) return;
    // if (pResult->data != NULL) free(pResult->data);

    if (num01.sign != num02.sign)
    {
        if (ubigger(num01, num02, 0) == 1)
        {
            pResult->sign = num01.sign;
            return _sub(pResult, num01, num02, 0);
        }
        else
        {
            pResult->sign = num02.sign;
            return _sub(pResult, num02, num01, 0);
        }
    }
    else
    {
        pResult->sign = num01.sign;
        return _add(pResult, num01, num02, 0);
    }
}

void subtract(sBigNum *pResult, const sBigNum num01, const sBigNum num02)
{
    if (pResult == NULL) return;
    // if (pResult->data != NULL) free(pResult->data);

    if (num01.sign != num02.sign)
    {
        pResult->sign = num01.sign;
        return _add(pResult, num01, num02, 0);
    }
    else
    {
        if (ubigger(num01, num02, 0) == 1)
        {
            pResult->sign = num01.sign;
            return _sub(pResult, num01, num02, 0);
        }
        else
        {
            pResult->sign = 1 - num02.sign;
            return _sub(pResult, num02, num01, 0);
        }
    }
}

void multiply(sBigNum *pResult, const sBigNum num01, const sBigNum num02)
{
    if (pResult == NULL) return;
    // if (pResult->data != NULL) free(pResult->data);

    pResult->sign = num01.sign ^ num02.sign;

    pResult->size = 0;
    pResult->data = (uint8_t*) malloc((num01.size + num02.size) * sizeof(uint8_t));
    if (pResult->data == NULL) return;  // run out of memory.
    pResult->size = (num01.size + num02.size);

    for (uint32_t i = 0; i < pResult->size; ++i)  // initial
    {
        pResult->data[i] = 0;
    }

    sBigNum num03;

    num03.sign = 0;
    num03.size = 0;
    num03.data = (uint8_t*) malloc((num01.size + 1) * sizeof(uint8_t));
    if (num03.data == NULL) return;  // run out of memory.
    num03.size = num01.size + 1;

    for (uint32_t i = 0; i < num02.size; ++i)  // multiply
    {
        for (uint32_t j = 0; j < num03.size; ++j)  // initial
        {
            num03.data[j] = 0;
        }

        num03.data[0] = ((uint16_t) num01.data[0]) * ((uint16_t) num02.data[i]) % 100;
        for (uint32_t j = 1; j < num01.size; ++j)
        {
            num03.data[j] = ((uint16_t) num01.data[j]) * ((uint16_t) num02.data[i]) % 100;
            num03.data[j] += ((uint16_t) num01.data[j-1]) * ((uint16_t) num02.data[i]) / 100;
        }

        for (uint32_t j = 0; j+1 < num03.size; ++j)  // carry
        {
            num03.data[i+1] = num03.data[i+1] + num03.data[i] / 100;
            num03.data[i] = num03.data[i] % 100;
        }

        _add(pResult, *pResult, num03, i);
    }

    uint32_t unused = 0;  // remove unused
    for (uint32_t i = 0; i+1 < pResult->size; ++i)
    {
        if (pResult->data[pResult->size-1-i] == 0) unused = unused + 1;
        else break;
    }

    pResult->size = pResult->size - unused;
    pResult->data = (uint8_t*) realloc(pResult->data, pResult->size*sizeof(uint8_t));

    if (pResult->data[pResult->size-1] == 0) pResult->sign = 0;  // deal with -0 issue.
}

void divide(sBigNum *pQuotient, sBigNum *pRemainder, const sBigNum num01, const sBigNum num02)
{
    if (pQuotient == NULL || pRemainder == NULL) return;
    // if (pQuotient->data != NULL) free(pQuotient->data);
    // if (pRemainder->data != NULL) free(pRemainder->data);

    if (num02.size > num01.size)  // quotient = 0
    {
        pQuotient->sign = 0;
        pRemainder->sign = num01.sign ^ num02.sign;

        pQuotient->size = 0;
        pQuotient->data = (uint8_t*) malloc(sizeof(uint8_t));
        if (pQuotient->data == NULL) return;  // run out of memory.
        pQuotient->size = 1;

        pQuotient->data[0] = 0;

        pRemainder->size = 0;
        pRemainder->data = (uint8_t*) malloc(num01.size * sizeof(uint8_t));
        if (pRemainder->data == NULL) return;  // run out of memory.
        pRemainder->size = num01.size;

        for (uint32_t i = 0; i < num01.size; ++i)
        {
            pRemainder->data[i] = num01.data[i];
        }
    }

    else
    {
        // sBigNum pTemp;

        // pQuotient->sign = num01.sign ^ num02.sign;
        // pRemainder->sign = num01.sign ^ num02.sign;

        // pQuotient->size = 0;
        // pQuotient->data = (uint8_t*) malloc((num01.size - num02.size + 1) * sizeof(uint8_t));
        // if (pQuotient->data == NULL) return;  // run out of memory.
        // pQuotient->size = (num01.size - num02.size + 1);

        // pRemainder->size = 0;
        // pRemainder->data = (uint8_t*) malloc(num01.size * sizeof(uint8_t));
        // if (pRemainder->data == NULL) return;  // run out of memory.
        // pRemainder->size = num01.size;

        // pTemp.size = 0;
        // pTemp.data = (uint8_t*) malloc((num02.size + 1) * sizeof(uint8_t));
        // if (pTemp.data == NULL) return;  // run out of memory.
        // pTemp.size = (num02.size + 1);

        // for (uint32_t i = 0; i < pQuotient->size; ++i)
        // {
        //     pQuotient->data[i] = 0;
        // }

        // for (uint32_t i = 0; i < pRemainder->size; ++i)
        // {
        //     pRemainder->data[i] = num01.data[i];
        // }

        // for (uint32_t i = 0; i < num02.size; ++i)
        // {

        //     uint32_t buffer = 0;
        //     // printf("D= %d", num02.data[i]);
        //     for (uint32_t k = 6; k + 1 > 0; --k)
        //     {
        //         for (uint32_t j = 0; j < pTemp.size; ++j)  // initial
        //         {
        //             pTemp.data[j] = 0;
        //         }

        //         pTemp.data[0] = ((uint16_t) num02.data[0]) * ((uint16_t) k) % 100;  // multiply
        //         for (uint32_t j = 1; j < num02.size; ++j)
        //         {
        //             pTemp.data[j] = ((uint16_t) num02.data[j]) * ((uint16_t) k) % 100;
        //             pTemp.data[j] += ((uint16_t) num02.data[j-1]) * ((uint16_t) k) / 100;
        //         }

        //         for (uint32_t j = 0; j+1 < pTemp.size; ++j)  // carry
        //         {
        //             pTemp.data[i+1] = pTemp.data[i+1] + pTemp.data[i] / 100;
        //             pTemp.data[i] = pTemp.data[i] % 100;
        //         }
        //         if (num02.data[i] >= buffer + (1 << j))
        //         {
        //             buffer = buffer + (1 << j);
        //         }
        //         printf(" %d ", (1 << j));
        //     }
        //     printf("\n");
        //     printf("%d\n", buffer);
        // }

    }
}

int32_t power(sBigNum *pResult, int32_t n, int32_t k)
{
    if (pResult == NULL) return 1;
    // if (pResult->data != NULL) free(pResult->data);

    return 0;
}

int32_t combine(sBigNum *pResult, int32_t n, int32_t k)
{
    if (pResult == NULL) return 1;
    // if (pResult->data != NULL) free(pResult->data);
    return 0;
}
