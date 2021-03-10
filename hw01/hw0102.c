#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"

#define DEFAULT_COLOR WHITE
#define ORIGINAL_COLOR BLUE
#define REPLACEMRNT_COLOR RED

#define N 1024
#define M 128

typedef struct _sColors
{
    int pos;
    const char *code;
    struct _sColors *next;
} sColors;

void AddColors(sColors **destination, const int position, const char * const colorcode);
void FreeColors(sColors **destination);

void ColorfulPrint(const char* string, const sColors *cdata);
void ColorfulReplacement(sColors **repleacementcolor, sColors **originalcolor, char *destination, const char *source, const char *keyword, const char *newword);

int main()
{
    char ibuffer[N], obuffer[N], keyword[M], newword[M];
    sColors *original_color = NULL, *new_color = NULL;

    printf("Please enter the original text:\n");
    fgets(ibuffer, N, stdin);
    printf("Keyword:\n");
    fgets(keyword, M, stdin);
    printf("New word:\n");
    fgets(newword, M, stdin);

    if (keyword[strlen(keyword)-1] == 0x0A) keyword[strlen(keyword)-1] = 0x00;  // remove '\n'
    if (newword[strlen(newword)-1] == 0x0A) newword[strlen(newword)-1] = 0x00;  // remove '\n'

    ColorfulReplacement(&new_color, &original_color, obuffer, ibuffer, keyword, newword);

    printf("\nBefore:\n");
    ColorfulPrint(ibuffer, original_color);

    printf("After:\n");
    ColorfulPrint(obuffer, new_color);

    FreeColors(&original_color);
    FreeColors(&new_color);
    FreeColors(&original_color);
    FreeColors(&new_color);

    return 0;
}

void AddColors(sColors **d, const int p, const char * const c)
{
    if (d == NULL) return;
    while (*d != NULL)
    {
        d = &((*d)->next);
    }
    (*d) = (sColors*) malloc(sizeof(sColors));
    (*d)->pos = p;
    (*d)->code = c;
    (*d)->next = NULL;
}

void FreeColors(sColors **destination)
{
    if (destination == NULL || *destination == NULL) return;
    while (*destination != NULL)
    {
        sColors *p = *destination;
        *destination = (*destination)->next;
        free(p);
    }
}

void ColorfulPrint(const char* str, const sColors *cdata)
{
    int idx = 0;
    while (str != NULL && str[idx] != 0x00)
    {
        while (cdata != NULL && cdata->pos == idx)
        {
            printf("%s", cdata->code);
            cdata = cdata->next;
        }
        printf("%c", str[idx]);
        idx++;
    }
}

void ColorfulReplacement(sColors **r, sColors **o, char *d, const char *s, const char *k, const char *n)
{
    if (s == NULL || k == NULL || n == NULL || r == NULL || o == NULL) return;

    AddColors(o, 0, DEFAULT_COLOR);
    AddColors(r, 0, DEFAULT_COLOR);

    const int slen = strlen(s);
    const int olen = strlen(k);
    const int nlen = strlen(n);

    int idx = 0, idy = 0;
    char *p = strstr(s, k);
    while (p != NULL)
    {
        strncpy(d+idy, s+idx, p-s-idx);
        idy += p-s-idx;
        idx += p-s-idx;

        AddColors(o, idx, ORIGINAL_COLOR);
        AddColors(o, idx+olen, DEFAULT_COLOR);
        AddColors(r, idy, REPLACEMRNT_COLOR);
        AddColors(r, idy+nlen, DEFAULT_COLOR);

        strncpy(d+idy, n, nlen+1);
        idx += olen;
        idy += nlen;

        p = strstr(p+olen, k);
    }

    if (idx < slen)
    {
        strncpy(d+idy, s+idx, slen-idx);
        idy += slen-idx;
        idx += slen-idx;
    }
    d[idy] = '\0';
}
