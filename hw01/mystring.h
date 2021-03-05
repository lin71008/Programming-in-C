#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include <string.h>

// char* mystrchr(const char *s, int c);
// char* mystrrchr(const char *s, int c);
// size_t mystrspn(const char *s, const char *accept);
// size_t mystrcspn(const char *s, const char *reject);
// char* mystrpbrk(const char *s, const char *accept);
// char* mystrstr(const char *haystack , const char *needle);
// char* mystrtok(char *str, const char *delim);

#define mystrchr strchr
#define mystrrchr strrchr
#define mystrspn strspn
#define mystrcspn strcspn
#define mystrpbrk strpbrk
#define mystrstr strstr
#define mystrtok strtok

#endif
