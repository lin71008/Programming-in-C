#include "myprintf.h"

int myprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int flag = 0;

    for (const char *p = format; *p != '\0'; ++p)
    {
        if (*p == '%' && flag == 0)
        {
            flag = 1;
            continue;
        }
        if (flag == 1)  // %%, %s, %c, ...
        {
            if (*p == '%')
            {
                fputc('%', stdout);
            }
            else if (*p == 'c')
            {
                char c = va_arg(args, int);
                fputc(c, stdout);
            }
            else if (*p == 'd')
            {
                int i = va_arg(args, int);
                char buffer[21];
                itoa(i, buffer, 10);
                for (const char *c = buffer; *c != '\0'; ++c)
                {
                    fputc(*c, stdout);
                }
            }
            else if (*p == 'i')
            {
                int i = va_arg(args, int);
                char buffer[21];
                itoa(i, buffer, 10);
                for (const char *c = buffer; *c != '\0'; ++c)
                {
                    fputc(*c, stdout);
                }
            }
            else if (*p == 'x')
            {
                int i = va_arg(args, int);
                char buffer[21];
                itoa(i, buffer, 16);
                for (const char *c = buffer; *c != '\0'; ++c)
                {
                    fputc(*c, stdout);
                }
            }
            else if (*p == 'X')
            {
                int i = va_arg(args, int);
                char buffer[21];
                itoa(i, buffer, 16);
                for (const char *c = buffer; *c != '\0'; ++c)
                {
                    if ('0' <= *c && *c <= '9')
                    {
                        fputc(*c, stdout);
                    }
                    else  // lower -> upper
                    {
                        fputc(((*c) ^ 0x20), stdout);
                    }
                }
            }
            else if (*p == 's')
            {
                const char *s = va_arg(args, const char*);
                for (const char *c = s; *c != '\0'; ++c)
                {
                    fputc(*c, stdout);
                }
            }
            flag = 0;
        }
        else  // "str"
        {
            fputc(*p, stdout);
        }
    }

    va_end(args);

    return 0;
}
