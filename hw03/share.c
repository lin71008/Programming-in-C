#include "share.h"

void remove_newline(char *s)
{
    if (s == NULL) return;
    if (s[strlen(s)-1] == '\n') s[strlen(s)-1] = '\0';
}

char* file_extension(const char *s)
{
    char *c = strrchr(s, '.');
    if (c != NULL && *c != '\0') return c+1;
    else return NULL;
}
