#include <stdio.h>

#include "mixed.h"

int main()
{
    sMixedNumber a, b, ans;

    mixed_set(&a, 0, 13, 24);
    mixed_set(&b, 0, 17, 24);

    mixed_print(a);
    mixed_print(b);

    mixed_add(&ans, a, b);
    mixed_print(ans);

    mixed_sub(&ans, a, b);
    mixed_print(ans);

    mixed_set(&a, 9, 9, 10);
    mixed_set(&b, 5, 5, 21);

    mixed_mul(&ans, a, b);
    mixed_print(ans);

    mixed_div(&ans, a, b);
    mixed_print(ans);

    return 0;
}
