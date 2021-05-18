#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#include "myprintf.h"

int myprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    const char *pstr = NULL;
    int32_t sign = 0, base = 10, num = 0;
    char buffer[100] = {0};
    int pre_zero, pre_sign, a, b, crt = 0;

    const char *lower_hex = "0123456789abcdef";
    const char *upper_hex = "0123456789ABCDEF";
    const char *use_hex = NULL;
    use_hex = lower_hex;

    for (const char *p = format; (p != NULL && *p != '\0'); ++p)
    {
        if (*p == '%')
        {
            ++p;

            pre_zero = 0, pre_sign = 0, a = 0, b = 0, crt = 0;
            pstr = NULL, sign = 0, base = 10, use_hex = lower_hex;

            if (*p == '+') { pre_sign = 1; ++p; }
            if (*p == '0') { pre_zero = 1; ++p; }
            for (; ('0' <= *p && *p <= '9'); ++p)
            {
                a = a * 10 + ((*p) - '0');
            }
            if (*p == '.') { ++p; }
            for (; ('0' <= *p && *p <= '9'); ++p)
            {
                b = b * 10 + ((*p) - '0');
            }

            switch (*p)
            {

// escape character

                case '%':
                    fputc('%', stdout);
                    break;

// character, string

                case 'c':
                    for (int i = 0; i < (a - 1); ++i)
                    {
                        putc(' ', stdout);
                    }
                    fputc(va_arg(args, int), stdout);
                    break;

                case 's':
                    pstr = va_arg(args, const char*);
                    for (const char *c = pstr; *c != '\0'; ++c, ++crt) { /* do nothing, just counting string length */ }
                    for (int i = 0; i < (a - crt); ++i)
                    {
                        putc(' ', stdout);
                    }
                    for (const char *c = pstr; *c != '\0'; ++c)
                    {
                        fputc(*c, stdout);
                    }
                    break;

// integer

                case 'o':
                    base = 8;
                    goto L1;

                case 'x':
                    base = 16;
                    goto L1;

                case 'X':
                    base = 16;
                    use_hex = upper_hex;
                    goto L1;

                case 'i':
                case 'd':
                    base = 10;
L1:
                    num = va_arg(args, int32_t);
                    if (num < 0)
                    {
                        num = -num;
                        sign = 1;
                    }
                    for (; num > 0; ++crt)
                    {
                        buffer[crt] = use_hex[num % base];
                        num /= base;
                    }
                    if (pre_zero == 0)
                    {
                        for (int i = 0; i < a - (crt + (sign | pre_sign)); ++i)
                        {
                            putc(' ', stdout);
                        }
                        if (sign == 1) { putc('-', stdout); }
                        if (pre_sign == 1 && sign == 0) { putc('+', stdout); }

                        for (int i = crt - 1; i >= 0; --i)
                        {
                            putc(buffer[i], stdout);
                        }
                    }
                    else
                    {
                        if (sign == 1) { putc('-', stdout); }
                        if (pre_sign == 1 && sign == 0) { putc('+', stdout); }
                        for (int i = 0; i < a - (crt + (sign | pre_sign)); ++i)
                        {
                            putc('0', stdout);
                        }

                        for (int i = crt - 1; i >= 0; --i)
                        {
                            putc(buffer[i], stdout);
                        }
                    }
                    break;

// end of line

                case '\0':
                    goto EXIT;
                    break;

// unknown error / unsupported format

                default:
                    fputc('%', stdout);
                    fputc(*p, stdout);
            }
        }

// normal

        else
        {
            fputc(*p, stdout);
        }
    }

EXIT:

    va_end(args);

    return 0;
}
