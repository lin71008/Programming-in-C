#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>

#ifdef _WIN32
    // sleep
    #include <unistd.h>
    // color
    #include <windows.h>
#else
    // sleep
    #include <sys/time.h>
    // color using ANSI
#endif

#include "share.h"
#include "color.h"

// lyrics color setting
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

void set_color(int32_t n)
{
    #ifdef _WIN32
        HANDLE  hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (n == -1)
        {
            SetConsoleTextAttribute(hConsole, 15);
        }
        else if (n < LyricsColor_MAX)
        {
            SetConsoleTextAttribute(hConsole, n+1);
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
        struct timespec sDelay = {.tv_sec = (delay / 100), .tv_nsec = (delay % 100) * 1000};
        while (nanosleep(&sDelay, &sDelay) == -1 && errno != EINTR) { /* waiting */ }
    #endif
}

int main()
{
    char FileName[256] = {0};

    printf("Open a LRC file: ");
    fgets(FileName, 256, stdin);
    remove_newline(FileName);

    if (file_extension(FileName) == NULL || strncmp("lrc", file_extension(FileName), 4) != 0)
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

    time_t cur = clock();
    uint32_t m = 0, s = 0, x = 0;  // current lyrics time
    uint32_t lm = 0, ls = 0, lx = 0;  // last lyrics time

    char *name_buffer[LyricsColor_MAX];
    int32_t top = 0;
    uint8_t flag = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char lyrics[read];
        if (sscanf(line, "[%d:%d.%d]%[^\n]", &m, &s, &x, lyrics) == 4)
        {
            int32_t delay = (60000*m+1000*s+x)-(60000*lm+1000*ls+lx);  // [ms]
            sleep_ms(delay);
            remove_newline(lyrics);
            printf("[%02ld:%02ld.%02ld]: %s\n", (clock()-cur)/CLOCKS_PER_SEC/60, ((clock()-cur)/CLOCKS_PER_SEC)%60,((clock()-cur)/CLOCKS_PER_SEC*100)%100, lyrics);
            lm = m, ls = s, lx = x;
        }
        else if (line[read-2] == ':')
        {
            flag = 1;
            for (int32_t i = 0; i < top && i < LyricsColor_MAX; ++i)
            {
                if (strcmp(line, name_buffer[i]) == 0)
                {
                    set_color(i);
                    flag = 0;
                    break;
                }
            }
            if (flag == 1)
            {
                name_buffer[top++] = strdup(line);
                set_color(top-1);
            }
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
