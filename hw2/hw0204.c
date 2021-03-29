#include <stdio.h>

#include "bignum.h"

int main()
{
    sBigNum a, b;

    set(&a, "123");
    set(&b, "99");

    print(a);
    print(b);

    if (compare(a, b) == 1)
    {
        printf("Comparison Pass.\n");
    }
    else
    {
        printf("Comparison Fail.\n");
    }

    if(digits(a) == 3)
    {
        printf("Digits Pass.\n");
    }
    else
    {
        printf("Digits Fail.\n");
    }

    sBigNum ans, q, r;

    add(&ans, a, b);
    print(ans);

    subtract(&ans, a, b);
    print(ans);

    multiply(&ans, a, b);
    print(ans);

    // divide(&q, &r, a, b);
    // print(q);
    // print(r);

    // power(&ans, 20, 10);
    // print(ans);

    // combine(&ans, 20, 10);
    // print(ans);

    return 0;
}
