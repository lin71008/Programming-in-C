#include <stdio.h>

#include "myprintf.h"

#define print(format, args...) printf("printf:   "format, ##args);\
    myprintf("myprintf: "format, ##args);\

int main()
{
    print("%%d: %3d %03d %+3d %+03d\n", 1, 2, 3, 4);
    print("%%i: %3i %03i %+3i %03i\n", -5, -6, -7, -8);
    print("%%o: %3o %03o %3o %03o\n", 5, 6, 7, 8);
    print("%%x: %3x %03x %3x %03x\n", 10, 11, 12, 13);
    print("%%X: %3X %03X %3X %03X\n", 14, 15, 16, 17);
    print("%%s: %7s %s\n", "Hello", "World !");
    print("%%c: %7c %c\n", 'A', '!');
    return 0;
}
