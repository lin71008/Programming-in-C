#include <stdio.h>

typedef union
{
    struct
    {
        unsigned char b1: 1;
        unsigned char b2: 1;
        unsigned char b3: 1;
        unsigned char b4: 1;
        unsigned char b5: 1;
        unsigned char b6: 1;
        unsigned char b7: 1;
        unsigned char b8: 1;
    } bits;
    unsigned char byte;
} uByte;

void uByte_print(const uByte b);
void uByte_flip(uByte *b, const int idx);

int main()
{
    uByte X;
    unsigned int idx;
    printf("Please enter a byte (0-255): ");
    scanf("%d", (int*)&X.byte);

    uByte_print(X);

    printf("Flip bit (1-8, 0: exit): ");
    scanf("%d", &idx);

    while (idx != 0)
    {
        uByte_flip(&X, idx);
        uByte_print(X);

        printf("Flip bit (1-8, 0: exit): ");
        scanf("%d", &idx);
    }

    printf("Bye\n");
    return 0;
}

void uByte_print(const uByte b)
{
    printf("Data: %3u %u%u%u%u%u%u%u%u\n", b.byte, b.bits.b8, b.bits.b7, b.bits.b6, b.bits.b5, b.bits.b4, b.bits.b3, b.bits.b2, b.bits.b1);
}

void uByte_flip(uByte *b, const int idx)
{
    switch (idx)
    {
        case 1:
            b->bits.b8 = b->bits.b8 ^ 1;
        break;
        case 2:
            b->bits.b7 = b->bits.b7 ^ 1;
        break;
        case 3:
            b->bits.b6 = b->bits.b6 ^ 1;
        break;
        case 4:
            b->bits.b5 = b->bits.b5 ^ 1;
        break;
        case 5:
            b->bits.b4 = b->bits.b4 ^ 1;
        break;
        case 6:
            b->bits.b3 = b->bits.b3 ^ 1;
        break;
        case 7:
            b->bits.b2 = b->bits.b2 ^ 1;
        break;
        case 8:
            b->bits.b1 = b->bits.b1 ^ 1;
        break;
        default:
        break;
    }
}
