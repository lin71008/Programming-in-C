開發紀錄
=======
basic
-----
```c
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```
> getline() reads an entire line from stream, storing the address of the buffer containing the text into *lineptr. The buffer is null-terminated and includes the newline character, if one was found.

If *lineptr is NULL, then getline() will allocate a buffer for storing the line, which should be freed by the user program. (In this case, the value in *n is ignored.)

Alternatively, before calling getline(), *lineptr can contain a pointer to a malloc(3)-allocated buffer *n bytes in size. If the buffer is not large enough to hold the line, getline() resizes it with realloc(3), updating *lineptr and *n as necessary.

In either case, on a successful call, *lineptr and *n will be updated to reflect the buffer address and allocated size respectively.  -- from [getline(3)]

[getline(3)]: https://linux.die.net/man/3/getline

hw0301
------
```c
int flag = (strncmp("lrc", file_extension(FileName), 4) == 0);
```

> This function starts comparing the first character of each string. If they are equal to each other, it continues with the following pairs until the characters differ, until a terminating null-character is reached, or until num characters match in both strings, whichever happens first.  -- from [cplusplus]

Using `4` to prevent something like `lrc+`

=> compare `{'l', 'r', 'c', '\0'}` and `{'l', 'r', 'c', '+'}`

[cplusplus]: https://www.cplusplus.com/reference/cstring/strncmp


```c
int sscanf(const char *str, const char *format, ...);
```

> These functions return the number of input items successfully matched and assigned, which can be fewer than provided for, or even zero in the event of an early matching failure.  -- from [sscanf(3)]

[sscanf(3)]: https://linux.die.net/man/3/sscanf

用 `pthread` 或 `windows.h` 什麼的實作之後再說...

hw0302
------
CSV

hw0303 & hw0304
---------------
BMP

hw0305
------
研究檔案資料中...
NBDATA.S5 => 登陸武將
SAVEDATA.S5P => `R.P.@` 貌似是存檔開頭?
0x1ac0, 0xc7ee, 0x2224a, 0x2cf78, 0x37ca6
https://www.sanguogame.com.cn/special/san5/san5-x.html

hw0306
------
