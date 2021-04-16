#include <stdio.h>
#include <stdint.h>

void max(int32_t a, int32_t b)
{
    if (a > b) printf("%d\n", a);
    else printf("%d\n", b);
}

int main()
{
    max(100, 50);
    max(50, 100);

    max(-100, -50);
    max(-50, -100);

    max(100, -50);
    max(-50, 100);

    max(-100, 50);
    max(50, -100);

    max(INT32_MAX, 0);
    max(0, INT32_MAX);

    max(INT32_MIN, 0);
    max(0, INT32_MIN);

    max(INT32_MIN, INT32_MAX);
    max(INT32_MAX, INT32_MIN);

    return 0;
}
