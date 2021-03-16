#include <stdio.h>
#include <ctype.h>

#include "status.h"

#define ONELINE
#define N 80

static const char* _lStatusStr[] = {
    "No Error",
    "Given argument can't be NULL",
    "The hex string should be ended with 00",
    "The length of the hex string should be a multiple of two",
    "The hex-string contain illegal character",
    "All input characters must be printable",
    "Unknown Error"
};

eStatus HexToStr(char *destination, const int len, const char *source);

int main()
{
    char ibuffer[2*N+1], obuffer[N+1] = {0};
    eStatus s;

    while(fgets(ibuffer, 2*N+1, stdin) != NULL)
    {
        s = HexToStr(obuffer, N, ibuffer);
        printf("%s", obuffer);
        if (s != Success && s != Continue)
        {
            printf("Error: %s\n", eStatusStr(s));
            break;
        }
        if (s == Success)
        {
            printf("\n");
#ifdef ONELINE
            break;
#endif
        }
    }
    return 0;
}

const char* eStatusStr(const eStatus e)
{
    if (e == Success || e == Continue) return _lStatusStr[0];
    else if (e == InvalidNull) return _lStatusStr[1];
    else if (e == ErrorFormat_0) return _lStatusStr[2];
    else if (e == ErrorFormat_1) return _lStatusStr[3];
    else if (e == InvalidCharacter_0) return _lStatusStr[4];
    else if (e == InvalidPatten_0) return _lStatusStr[5];
    else return _lStatusStr[6];
}

static inline int HexcharToHex(const char c)
{
    return isdigit(c)?c-0x30:(c|0x20)-0x57;
}

static char last = '\0';
eStatus HexToStr(char *d, const int len, const char *s)
{
    if (d == NULL || s == NULL) return InvalidNull;

    int idx = 0;
    char L, H = s[2*idx];
    if (H == '\0' || H == '\n') return Success;
    while (H != '\0' && idx < len)
    {
        H = s[2*idx]; L = s[2*idx+1];
        if (H == '\n' || H == '\0')
        {
            if (last == '\0')
            {
                d[idx] = '\0';
                return Success;
            }
            else
            {
                last = '\0';
                d[idx] = '\0';
                return ErrorFormat_0;
            }
        }
        else if (L == '\n' || L == '\0')
        {
            last = '\0';
            d[idx] = '\0';
            return ErrorFormat_1;
        }
        else if (isxdigit(H) && isxdigit(L))
        {
            last = HexcharToHex(L)|HexcharToHex(H)<<4;
            if (isprint(last) || last == '\0' || last == '\t' || last == '\n')
            {
                d[idx] = last;
            }
            else
            {
                last = '\0';
                d[idx] = '\0';
                return InvalidPatten_0;
            }
        }
        else
        {
            last = '\0';
            d[idx] = '\0';
            return InvalidCharacter_0;
        }
        idx++;
    }
    return Continue;
}
