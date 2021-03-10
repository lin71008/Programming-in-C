#include <stdio.h>
#include <ctype.h>

#define ONELINE
#define N 8

int HexToStr(char *destination, const int len, const char *source);

int main()
{
    int endline = 0;
    char ibuffer[2*N+1], obuffer[N+1] = {0};

    while(fgets(ibuffer, 2*N+1, stdin) != NULL)
    {
        endline = HexToStr(obuffer, N, ibuffer);
        printf("%s", obuffer);
        if (endline)
        {
            printf("\n");
#ifdef ONELINE
            break;
#endif
        }
    }
    return 0;
}

static inline int hexchartohex(const char c)
{
    return isdigit(c)?c-0x30:(c|0x20)-0x57;
}

static char last = '\0';
int HexToStr(char *d, const int len, const char *s)
{
    if (d == NULL || s == NULL) return 0;
    int idx = 0;
    char L, H = s[2*idx];
    while (H != '\0' && idx < len)
    {
        H = s[2*idx]; L = s[2*idx+1];
        if (H == '\n')
        {
            if (last != '\0')  // error: hex-string missing tail '\0'
            {
                printf("Error: The hex string should be ended with 00.\n");
            }
            d[idx] = '\0';
            last = '\0';
            return 1;
        }
        else if (L == '\n' || L == '\0')  // error: hex-string missing one byte
        {
            printf("Error: The length of the hex string should be a multiple of two.\n");
            last = '\0';
            break;
        }
        else if (isxdigit(H) && isxdigit(L))
        {
            last = hexchartohex(L)|hexchartohex(H)<<4;
            if (last == '\0' || last == '\t' || last == '\n' || isprint(last))
            {
                d[idx] = last;
            }
            else  // error: invalid character.
            {
                printf("Error: All input characters must be printable.\n");
                last = '\0';
                break;
            }
        }
        else  // error: hex-string contain illegal character
        {
            printf("Error: The hex-string contain illegal character.\n");
            last = '\0';
            break;
        }
        idx++;
    }
    d[idx] = '\0';
    return 0;
}
