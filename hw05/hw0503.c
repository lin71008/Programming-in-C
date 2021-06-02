#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>

#include <curl/curl.h>

static int flag_q = 0;
static int flag_help = 0;
static int flag_error = 0;

static const struct option long_options[] =
{
    { "help", 0, NULL, 'h' },
    { 0,      0,    0,  0  }
};

static void usage()
{
    printf("Usage:    ./hw0503 -q <target>\n");
    printf("Search DBLP and show the first ten papers of the query result.\n");
}

int main(int argc, char **argv)
{
    int32_t option = 0;
    char target[100] = {0};
    char search_url[200] = "https://dblp.org/search?q=";
    while ((option = getopt_long(argc, argv, "hq:", long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'h':
                flag_help = 1;
                break;
            case 'q':
                if (!flag_q)
                {
                    strncpy(target, optarg, 100);
                    flag_q = 1;
                }
                else
                {
                    flag_error = 1;
                }
                break;
            case '?':
            default:
                flag_error = 1;
                break;
        }
    }
    if (flag_help)
    {
        usage();
        return 0;
    }
    if (flag_error || !flag_q)
    {
        printf("Error: please run ./hw0503 --help to see the usage.\n");
        return 0;
    }

    // FILE *fp = tmpfile();
    FILE *fp = fopen("./a.json", "wb+");
    strcat(search_url, target);

    CURL *curl = NULL;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, search_url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return 0;
        }
        curl_easy_cleanup(curl);
    }
    else
    {
        printf("curl_easy_init() failed: %s\n", curl_easy_strerror(res));
        return 0;
    }

    fseek(fp, 0, SEEK_SET);

    char *line = NULL;  // line buffer
    size_t len = 0;  // buffer size
    int32_t read;  // line len

    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (strstr(line, "<!-- search results -->"))
        {
            int32_t result_conuter = 0;

            char *next_info = NULL, *info = strstr(line, "<cite class=\"data\" itemprop=\"headline\"><span itemprop=\"author\" itemscope itemtype=\"http://schema.org/Person\">");
            while (info != NULL && result_conuter < 10)
            {
                printf("Paper %02d:\n", result_conuter+1);
                next_info = strstr(info + 109, "<cite class=\"data\" itemprop=\"headline\"><span itemprop=\"author\" itemscope itemtype=\"http://schema.org/Person\">");

                int32_t author_counter = 0;
                char author_buffer[20][200] = {0};

                char *buffer = strstr(info, "<span itemprop=\"name\" title=\"");
                while (author_counter < 20 && buffer != NULL && (next_info == NULL || buffer < next_info))
                {
                    sscanf(buffer+29, "%[^\"]", author_buffer[author_counter]);

                    buffer = strstr(buffer + 29, "<span itemprop=\"name\" title=\"");
                    author_counter++;
                }

                char title[1000] = {0};
                char source[500] = {0};
                int32_t year = 0;

                buffer = strstr(info, "<span class=\"title\" itemprop=\"name\">");
                if (sscanf(buffer+36, "%[^<]</span>%[^<]<span itemprop=\"datePublished\">%d", title, source, &year) != 3)
                {
                    char c[100] = {0};
                    buffer = strstr(info, "<span itemprop=\"name\">");
                    sscanf(buffer+22, "%[^<]", c);
                    strcat(source, c);
                    strcat(source, " ");

                    buffer = strstr(info, "<span itemprop=\"volumeNumber\">");
                    if (sscanf(buffer+30, "%[^<]", c))
                    {
                        strcat(source, c);
                        strcat(source, ": ");
                    }

                    buffer = strstr(info, "<span itemprop=\"pagination\">");
                    if (sscanf(buffer+28, "%[^<]", c))
                    {
                        strcat(source, c);
                    }

                    buffer = strstr(info, "<span itemprop=\"datePublished\">");
                    sscanf(buffer+31, "%d", &year);
                }

                printf("\tTitle: %s/n", title);
                printf("\tAuthor: ");
                for (int32_t i = 0; i < author_counter; ++i)
                {
                    printf("%s", author_buffer[i]);
                    if (i != author_counter - 1)
                    {
                        printf(", ");
                    }
                    else
                    {
                        printf(".\n");
                    }
                }
                printf("\tSource: %s\n", source);
                printf("\tYear: %d\n", year);

                info = next_info;
                result_conuter++;
            }
            break;
        }
    }

    fclose(fp);

    return 0;
}
