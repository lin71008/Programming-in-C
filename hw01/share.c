#include <string.h>

#ifndef Padding
#define Padding " \t\r\n"
#endif

char* SkipPadding(const char *s)
{
    if (s == NULL) return (char*) s;
    else return (char*) s + strspn(s, Padding);
}
