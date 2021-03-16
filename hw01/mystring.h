#ifndef __MYSTRING_H__
#define __MYSTRING_H__

extern char* mystrchr(const char *s, int c);
extern char* mystrrchr(const char *s, int c);
extern size_t mystrspn(const char *s, const char *accept);
extern size_t mystrcspn(const char *s, const char *reject);
extern char* mystrpbrk(const char *s, const char *accept);
extern char* mystrstr(const char *haystack , const char *needle);
extern char* mystrtok(char *str, const char *delim);

#endif
