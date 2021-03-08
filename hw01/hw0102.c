#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE_COLOR "\x1b[0m"
#define RED_COLOR "\x1b[31m"
#define BLUE_COLOR "\x1b[34m"

#define N 1024
#define M 128

typedef struct colors
{
    int pos;
    const char *code;
    struct colors *next;
} colors;

void add_colors(colors **destination, const int position, const char *colorcode);

void colorful_print(const char* string, const colors *cdata);
void replacement_with_colors(colors **repleacementcolor, colors **originalcolor, char *destination, const char *source, const char *keyword, const char *newword);

int main(int argc, char const *argv[])
{
    char ibuffer[N], obuffer[N], keyword[M], newword[M];
    colors *original_color = NULL, *new_color = NULL;

    printf("Please enter the original text:\n");
    fgets(ibuffer, N, stdin);
    printf("Keyword:\n");
    fgets(keyword, M, stdin);
    printf("New word:\n");
    fgets(newword, M, stdin);

    if (keyword[strlen(keyword)-1] == 0x0A) keyword[strlen(keyword)-1] = 0x00;  // remove '\n'
    if (newword[strlen(newword)-1] == 0x0A) newword[strlen(newword)-1] = 0x00;  // remove '\n'

    replacement_with_colors(&new_color, &original_color, obuffer, ibuffer, keyword, newword);

    printf("\nBefore:\n");
    colorful_print(ibuffer, original_color);

    printf("After:\n");
    colorful_print(obuffer, new_color);

    return 0;
}

void add_colors(colors **d, const int p, const char *c)
{
    if (d == NULL) return;
    while (*d != NULL)
    {
        d = &((*d)->next);
    }
    (*d) = (colors*) malloc(sizeof(colors));
    (*d)->pos = p;
    (*d)->code = c;
    (*d)->next = NULL;
}


void colorful_print(const char* str, const colors *cdata)
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

void replacement_with_colors(colors **r, colors **o, char *d, const char *s, const char *k, const char *n)
{
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

        add_colors(o, idx, BLUE_COLOR);
        add_colors(o, idx+olen, WHITE_COLOR);

        add_colors(r, idy, RED_COLOR);
        add_colors(r, idy+nlen, WHITE_COLOR);

        strncpy(d+idy, n, nlen);

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
