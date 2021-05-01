#ifndef __SHARE_H__
#define __SHARE_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

// remove '\n' character at the end of string $s. (if exist)
extern void remove_newline(char *s);

// return a pointer to the position of file extension in $s.
// if can not find extension, return NULL.
// example 1: $s = "a.b", $p = &s[2]
// example 2: $s = "a."or "a", $p = NULL
extern char* file_extension(const char *s);

// return 1 if ...
//   1. $file_extension($f) == $s
//   2. $s == NULL
extern int32_t check_file_extension(const char *f, const char *s);

// usage example code:
// remove_newline(f), ... etc.
// if (check_file_extension(f, e) != 1)  // File format not recognized.
// if ((fp = fopen(f, m)) == NULL)  // strerror(errno).
// fcolse(f);  // do not forget close

// char *buf = NULL;
// size_t bufs = 0;
// ssize_t len;
// while ((len = getline(&buf, &bufs, fp)) != -1) ...


#endif
