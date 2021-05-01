#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct _sMixed
{
    int32_t a;
    int32_t b;
    int32_t c;
} sMixed;

void set (sMixed *num, char *str)
{
    char *n = strtok(str, " (,)");
    num->a = atoi(n);

    n  = strtok(NULL, " (,)");
    num->b = atoi(n);

    n  = strtok(NULL, " (,)");
    num->c = atoi(n);
}

void print(const sMixed num)
{
    printf("(%d, %d, %d)\n", num.a, num.b, num.c);
}

int32_t gcd(int32_t a, int32_t b)
{
    if (a == 0 || b == 0) return a | b;
    else if (a == 1 || b == 1) return 1;
    else if (a > b) return gcd(a % b, b);
    else return gcd(a, b % a);
}

sMixed improper(sMixed num)
{
    sMixed result = {0, 0, 0};
    if (num.a < 0 || (num.a == 0 && num.b < 0))
    {
        result.b = num.a * num.c - num.b;
        result.c = num.c;
    }
    else
    {
        result.b = num.a * num.c + num.b;
        result.c = num.c;
    }
    return result;
}

sMixed Mixed_Add(sMixed X, sMixed Y)
{
    if (X.c == 0) X.c = 1;
    if (Y.c == 0) Y.c = 1;

    sMixed Xi = improper(X);
    sMixed Yi = improper(Y);
    sMixed result = {0, 0, 0};

    result.b = Xi.b * Yi.c + Yi.b * Xi.c;
    result.c = Xi.c * Yi.c;

    int32_t g = gcd(abs(result.b), result.c);

    result.b /= g;
    result.c /= g;

    int32_t sign = 0;
    if (result.b < 0) sign = 1;

    result.a = abs(result.b) / result.c;
    result.b = abs(result.b) % result.c;

    if (sign == 1)
    {
        if (result.a != 0) result.a *= -1;
        else result.b *= -1;
    }

    if (result.b == 0) result.c = 0;

    return result;
}

sMixed Mixed_Sub(sMixed X, sMixed Y)
{
    if (X.c == 0) X.c = 1;
    if (Y.c == 0) Y.c = 1;

    sMixed Xi = improper(X);
    sMixed Yi = improper(Y);
    sMixed result = {0, 0, 0};

    result.b = Xi.b * Yi.c - Yi.b * Xi.c;
    result.c = Xi.c * Yi.c;

    int32_t g = gcd(abs(result.b), result.c);

    result.b /= g;
    result.c /= g;

    int32_t sign = 0;
    if (result.b < 0) sign = 1;

    result.a = abs(result.b) / result.c;
    result.b = abs(result.b) % result.c;

    if (sign == 1)
    {
        if (result.a != 0) result.a *= -1;
        else result.b *= -1;
    }

    if (result.b == 0) result.c = 0;

    return result;
}

sMixed Mixed_Mul(sMixed X, sMixed Y)
{
    if (X.c == 0) X.c = 1;
    if (Y.c == 0) Y.c = 1;

    sMixed Xi = improper(X);
    sMixed Yi = improper(Y);
    sMixed result = {0, 0, 0};

    result.b = Xi.b * Yi.b;
    result.c = Xi.c * Yi.c;

    int32_t g = gcd(abs(result.b), result.c);

    result.b /= g;
    result.c /= g;

    int32_t sign = 0;
    if (result.b < 0) sign = 1;

    result.a = abs(result.b) / result.c;
    result.b = abs(result.b) % result.c;

    if (sign == 1)
    {
        if (result.a != 0) result.a *= -1;
        else result.b *= -1;
    }

    if (result.b == 0) result.c = 0;

    return result;
}

sMixed Mixed_Div(sMixed X, sMixed Y)
{
    if (X.c == 0) X.c = 1;
    if (Y.c == 0) Y.c = 1;

    sMixed Xi = improper(X);
    sMixed Yi = improper(Y);
    sMixed result = {0, 0, 0};

    result.b = Xi.b * Yi.c;
    result.c = Xi.c * Yi.b;

    int32_t g = gcd(abs(result.b), result.c);

    result.b /= g;
    result.c /= g;

    int32_t sign = 0;
    if (result.b < 0) sign = 1;

    result.a = abs(result.b) / result.c;
    result.b = abs(result.b) % result.c;

    if (sign == 1)
    {
        if (result.a != 0) result.a *= -1;
        else result.b *= -1;
    }

    if (result.b == 0) result.c = 0;

    return result;
}

int main(void)
{
    char iBuffer[1024] = {0};

    printf("Q: ");
    fgets(iBuffer, 1024, stdin);
    if (iBuffer[strlen(iBuffer)-1] == '\n') iBuffer[strlen(iBuffer)-1] = '\0';

    size_t top = 0;
    char Operator_Stack[100];
    sMixed Number_Stack[100];
    sMixed Ans = {0, 0, 0};


    char *str = iBuffer, *p = iBuffer;
    while (*p != '\0')
    {
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/')
        {
            Operator_Stack[top] = *p;
            *p = '\0';
            set(&Number_Stack[top], str);
            top++;
            str = p+1;
        }
        p++;
    }

    set(&Number_Stack[top], str);
    top++;


    int8_t ContainMulDiv = 1;
    while (ContainMulDiv == 1)
    {
        ContainMulDiv = 0;
        for (size_t i = 0; i+1 < top; ++i)
        {
            if (Operator_Stack[i] == '*')
            {
                ContainMulDiv = 1;
                Number_Stack[i] = Mixed_Mul(Number_Stack[i], Number_Stack[i+1]);
                for (size_t j = i; j+2 < top; ++j)
                {
                    Operator_Stack[j] = Operator_Stack[j+1];
                    Number_Stack[j+1] = Number_Stack[j+2];
                }
                top--;
                break;
            }
            if (Operator_Stack[i] == '/')
            {
                ContainMulDiv = 1;
                Number_Stack[i] = Mixed_Div(Number_Stack[i], Number_Stack[i+1]);
                for (size_t j = i; j+2 < top; ++j)
                {
                    Operator_Stack[j] = Operator_Stack[j+1];
                    Number_Stack[j+1] = Number_Stack[j+2];
                }
                top--;
                break;
            }
        }
    }
    while (top > 1)
    {
        for (size_t i = 0; i+1 < top; ++i)
        {
            if (Operator_Stack[i] == '+')
            {
                Number_Stack[i] = Mixed_Add(Number_Stack[i], Number_Stack[i+1]);
                for (size_t j = i; j+2 < top; ++j)
                {
                    Operator_Stack[j] = Operator_Stack[j+1];
                    Number_Stack[j+1] = Number_Stack[j+2];
                }
                top--;
                break;
            }
            if (Operator_Stack[i] == '-')
            {
                Number_Stack[i] = Mixed_Sub(Number_Stack[i], Number_Stack[i+1]);
                for (size_t j = i; j+2 < top; ++j)
                {
                    Operator_Stack[j] = Operator_Stack[j+1];
                    Number_Stack[j+1] = Number_Stack[j+2];
                }
                top--;
                break;
            }
        }
    }

    Ans = Number_Stack[0];

    printf("A: ");
    print(Ans);

    return 0;
}
