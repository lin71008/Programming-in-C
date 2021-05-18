#ifndef __SHARE_H__
#define __SHARE_H__

// remove '\n' character at the end of string s. (if exist)
extern void remove_newline(char *s);

// return a pointer to character to the begin point of the file extension in string s.
// if extension not exist or s in NULL, return NULL.
// for example: s = "a.b", return &s[2]
//              s = "abc", return NULL
extern char* file_extension(const char *s);

// if s is not NULL, return true if filename exist and its extension is s.
// otherwwise, only check filename exist.
extern int check_file_extension(const char *filename, const char *s);

#endif
