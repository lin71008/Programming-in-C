#include <stdio.h>

#include "myprintf.h"

#define print(format, args...) printf("printf:   "format, ##args);\
    myprintf("myprintf: "format, ##args);

int main()
{
    print("%%d: %d %d %d %d\n", 11, 12, 13, 14);
    print("%%i: %d %d %d %d\n", 15, 16, 17, 18);
    print("%%x: %d %d %d %d\n", 19, 20, 21, 22);
    print("%%X: %x %x %X %X\n", 23, 24, 25, 26);
    print("%%s: %s %s\n", "Hello", "World !");
    return 0;
}
