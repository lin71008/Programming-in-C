#include <stdio.h>
#include <ctype.h>

#define N 16

static int newline = 0;
static char lasthexchar = 0x00;

static int hexchartohex(const char c);
void hextostr(char *destination, const int len, const char *source);

int main()
{
    char ibuffer[2*N+1] = {0}, obuffer[N+1] = {0};

    while(fgets(ibuffer, 2*N+1, stdin) != NULL)
    {
        hextostr(obuffer, N, ibuffer);
        printf("%s", obuffer);
        if (newline)
        {
            printf("\n");
            newline = 0;
        }
    }

    return 0;
}

static int hexchartohex(const char c)
{
    return isdigit(c)?c-0x30:(c|0x20)-0x57;
}

void hextostr(char *d, const int m, const char *s)
{
    int idx = 0;
    char L, H = s[2*idx];
    while (H != 0x00 && idx < m)
    {
        H = s[2*idx]; L = s[2*idx+1];
        if (H == 0x0A)
        {
            if (lasthexchar != 0x00)
            {
                printf("Error: The hex string should be ended with 00.\n");
            }
            lasthexchar = 0x00;
            newline = 1;
            break;
        }
        else if (L == 0x0A || L == 0x00)  // error: hex-string missing one byte
        {
            printf("Error: The length of the hex string should be a multiple of two.");
            lasthexchar = 0x00;
            break;
        }
        else if (isxdigit(H) && isxdigit(L))
        {
            lasthexchar = hexchartohex(L)|hexchartohex(H)<<4;
            if (isprint(lasthexchar) || lasthexchar == 0x00)
            {
                d[idx] = lasthexchar;
            }
            else  // error: invalid character.
            {
                printf("Error: All input characters must be printable.\n");
                lasthexchar = 0x00;
                break;
            }
        }
        else  // error: hex-string contain illegal character
        {
            printf("Error: The hex-string contain illegal character.\n");
            lasthexchar = 0x00;
            break;
        }
        idx++;
    }
    d[idx] = 0x00;
}
