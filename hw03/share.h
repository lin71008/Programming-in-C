#ifndef __SHARE_H__
#define __SHARE_H__

#include <string.h>

// remove '\n' character at the end of string $s. (if exist)
extern void remove_newline(char *s);

// return a pointer to the position of file extension in $s.
// if can not find extension, return NULL.
// example 1: $s = "a.b", $p = &s[2]
// example 2: $s = "a."or "a", $p = NULL
extern char* file_extension(const char *s);

#endif
