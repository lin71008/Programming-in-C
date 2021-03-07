#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include <stdio.h>
#include <string.h>

char* mystrchr(const char *s, int c)
{
    if (s == NULL) return NULL;
    while (*s != '\0')
    {
        if (*s == c) return (char*) s;
        s++;
    }
    return NULL;
}

char* mystrrchr(const char *s, int c)
{
    if (s == NULL) return NULL;
    const char *p = NULL;
    while (*s != '\0')
    {
        if (*s == c) p = s;
        s++;
    }
    return (char*) p;
}

size_t mystrspn(const char *s, const char *accept)
{
    if (s == NULL || accept == NULL) return 0;
    size_t n = 0;
    const char *p;
    while (*s != '\0')
    {
        p = accept;
        while (*p != '\0')
        {
            if (*s == *p) break;
            p++;
        }
        if (*s != *p) break;
        n++; s++;
    }
    return n;
}

size_t mystrcspn(const char *s, const char *reject)
{
    if (s == NULL || reject == NULL) return 0;
    size_t n = 0;
    const char *p;
    while (*s != '\0')
    {
        p = reject;
        while (*p != '\0')
        {
            if (*s == *p) break;
            p++;
        }
        if (p != NULL && *p != '\0') break;
        n++; s++;
    }
    return n;
}

char* mystrpbrk(const char *s, const char *accept)
{
    if (s == NULL || accept == NULL) return NULL;
    const char *p;
    while (*s != '\0')
    {
        p = accept;
        while (*p != '\0')
        {
            if (*s == *p) return (char*) s;
            p++;
        }
        s++;
    }
    return NULL;
}

char* mystrstr(const char *haystack , const char *needle)
{
    if (haystack == NULL || needle == NULL) return NULL;
    const char *ph, *pn;
    while (*haystack != '\0')
    {
        ph = haystack; pn = needle;
        while (*pn != '\0')
        {
            if (*ph == '\0' || *ph != *pn) break;
            ph++; pn++;
        }
        if (*pn == '\0') return (char*) haystack;
        haystack++;
    }
    return NULL;
}

static char *last = NULL;
char* mystrtok(char *str, const char *delim)
{
    if ((str == NULL && last == NULL) || delim == NULL) return NULL;
    if (str == NULL) str = last;
    char *s = str; const char *p;
    while (*s != '\0')
    {
        p = delim;
        while (*p != '\0')
        {
            if (*s == *p)
            {
                if (s == str)
                {
                    str++;
                    break;
                }
                else
                {
                    *s = '\0';
                    last = s+1;
                    return str;
                }
            }
            p++;
        }
        s++;
    }
    if (*s == '\0') last = NULL;
    return str;
}

#endif
