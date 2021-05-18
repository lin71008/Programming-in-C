#include <stdio.h>

#include "myprintf.h"

#define print(format, args...) printf("printf:   "format, ##args);\
    myprintf("myprintf: "format, ##args);

int main()
{
    print("%%d: %d %d %d %d\n", 11, 12, 13, 14);
    print("%%i: %i %i %i %i\n", 11, 12, 13, 14);
    print("%%x: %x %x %x %x\n", 23, 24, 25, 26);
    print("%%X: %x %x %X %X\n", 23, 24, 25, 26);
    print("%%s: %s %s\n", "Hello", "World !");
    return 0;
}
