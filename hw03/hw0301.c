#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

// linux/nanosleep
#include <sys/time.h>

// windows/sleep
#include <time.h>

#include "share.h"

// color
#ifdef _WIN32
#include <windows.h>
#else
#include "color.h"
#endif


// lyrics color setting
#ifdef _WIN32
const int32_t DefaultColor = 15;
const int32_t LyricsColor[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
const int32_t LyricsColor_MAX = sizeof(LyricsColor) / sizeof(int32_t);
#else
const char *DefaultColor = CLR_LWHI;
const char *LyricsColor[] = { CLR_LRED,\
                              CLR_LBLU,\
                              CLR_MAG,\
                              CLR_LGRE,\
                              CLR_YEL,\
                              CLR_LMAG,\
                              CLR_LCYA,\
                              CLR_GRE,\
                              CLR_WHI};
const int32_t LyricsColor_MAX = sizeof(LyricsColor) / sizeof(char*);
#endif

void set_color(int32_t n)
{
#ifdef _WIN32
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (n == -1)
    {
        SetConsoleTextAttribute(hConsole, DefaultColor);
    }
    else if (n < LyricsColor_MAX)
    {
        SetConsoleTextAttribute(hConsole, LyricsColor[n]);
    }
#else
    if (n == -1)
    {
        printf("%s", DefaultColor);
    }
    else if (n < LyricsColor_MAX)
    {
        printf("%s", LyricsColor[n]);
    }
#endif
}

void sleep_ms(int32_t delay)
{
#ifdef _WIN32
    sleep(delay / 1000);
#else
    struct timespec sDelay = {.tv_sec = (delay / 1000), .tv_nsec = (delay % 1000) * 1000};
    while (nanosleep(&sDelay, &sDelay) == -1 && errno != EINTR) { /* waiting */ }
#endif
}

int main()
{
    char FileName[256] = {0};

    printf("Open a LRC file: ");
    fgets(FileName, 256, stdin);
    remove_newline(FileName);

    if (check_file_extension(FileName, "lrc") == 0)
    {
        printf("Warning: File format not recognized.\n");
        return 0;
    }

    FILE *fp = NULL;
    if ((fp = fopen(FileName, "r")) == NULL)
    {
        printf("Error: File could not be opened.\n");
        return 0;
    }

    char *line = NULL;  // line buffer
    size_t len = 0;  // buffer size
    ssize_t read;  // line len

    uint32_t m = 0, s = 0, x = 0;  // current lyrics time
    uint32_t lm = 0, ls = 0, lx = 0;  // last lyrics time

    char *name_buffer[LyricsColor_MAX];
    int32_t top = 0;
    uint8_t flag = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char attr[read], value[read];
        char singer[read], lyrics[read];

        if (sscanf(line, "[%d:%d.%d]%[^\n]", &m, &s, &x, lyrics) == 4)
        {
            int32_t delay = (60000*m+1000*s+x)-(60000*lm+1000*ls+lx);  // [ms]
            sleep_ms(delay);
            remove_newline(lyrics);
            printf("%s\n", lyrics);
            lm = m, ls = s, lx = x;
        }
        else if (sscanf(line, "[%[^:]:%[^]]]", attr, value) == 2)
        {
            // attribute: value
        }
        else if (sscanf(line, "%[^:]", singer) == 1)
        {
            flag = 1;
            for (int32_t i = 0; i < top && i < LyricsColor_MAX; ++i)
            {
                if (strcmp(singer, name_buffer[i]) == 0)
                {
                    set_color(i);
                    flag = 0;
                    break;
                }
            }
            if (flag == 1)
            {
                name_buffer[top++] = strdup(singer);
                set_color(top-1);
            }
        }
        else
        {
            // lyrics without time
            printf("%s", line);
        }
    }

    set_color(-1);

    for (int32_t i = 0; i < top; ++i)
    {
        free(name_buffer[i]);
    }

    free(line);
    fclose(fp);

    return 0;
}
