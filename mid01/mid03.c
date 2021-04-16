#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#pragma pack(push)
#pragma pack(1)
typedef struct _sCSV
{
    FILE *file;
    size_t column_amount;
    char **column_name;
    int32_t offset;
} sCSV;
#pragma pack(pop)

void init(sCSV *csv, FILE *file)
{
    csv->file = file;
    csv->column_amount = 0;
    csv->column_name = NULL;
    csv->offset = 0;

    fseek(csv->file, 0, SEEK_SET);

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, csv->file);

    if (nread == -1) return;
    char *name = strtok(line, ",");
    while(name != NULL)
    {
        csv->column_name = (char**) realloc(csv->column_name, sizeof(char*) * (csv->column_amount + 1));
        csv->column_name[csv->column_amount] = (char*) malloc(sizeof(char) * (strlen(name) + 1));
        strncpy(csv->column_name[csv->column_amount], name, (strlen(name) + 1));
        csv->column_amount++;
        name = strtok(NULL, ",");
    }

    csv->offset = ftell(csv->file);
}


time_t atot(const char *c)
{
    struct tm tmdate = {0};
    tmdate.tm_year = atoi(&c[0]) - 1900;
    tmdate.tm_mon = atoi(&c[5]) - 1;
    tmdate.tm_mday = atoi(&c[8]);
    time_t t = mktime(&tmdate);
    return t;
}

void Functional_1(sCSV csvfile)
{
    time_t start, end;
    char buffer[1024] = {0};

    printf("Start time: ");
    fgets(buffer, 1024, stdin);
    if (buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = '\0';
    start = atot(buffer);

    printf("End   time: ");
    fgets(buffer, 1024, stdin);
    if (buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = '\0';
    end = atot(buffer);

    printf("Answer: \n");
    int32_t flag[csvfile.column_amount];
    memset(flag, 0, sizeof(flag));

    for (size_t i = 1; i < csvfile.column_amount; ++i)
    {
        printf("Find [%s]", csvfile.column_name[i]);

        char *line = NULL;
        size_t len = 0;
        ssize_t nread;

        fseek(csvfile.file, csvfile.offset, SEEK_SET);
        while ((nread = getline(&line, &len, csvfile.file)) != -1)
        {
            time_t t = atot(line);
            if (start <= t && t <= end)
            {
                char *value = strtok(line, ",");
                while (value != NULL)
                {
                    printf("(%s), ", value);
                    value = strtok(NULL, ",");
                }
            }
        }
    }
}

void Functional_2()
{
    char StartTime[1024] = {0};
    char EndTime[1024] = {0};

    printf("Start time: ");
    fgets(StartTime, 1024, stdin);
    if (StartTime[strlen(StartTime)-1] == '\n') StartTime[strlen(StartTime)-1] = '\0';

    printf("End   time: ");
    fgets(EndTime, 1024, stdin);
    if (EndTime[strlen(EndTime)-1] == '\n') EndTime[strlen(EndTime)-1] = '\0';

    printf("Answer: \n");
}

void Functional_3()
{
    char StartTime[1024] = {0};
    char EndTime[1024] = {0};

    printf("Start time: ");
    fgets(StartTime, 1024, stdin);
    if (StartTime[strlen(StartTime)-1] == '\n') StartTime[strlen(StartTime)-1] = '\0';

    printf("End   time: ");
    fgets(EndTime, 1024, stdin);
    if (EndTime[strlen(EndTime)-1] == '\n') EndTime[strlen(EndTime)-1] = '\0';

    char City01[1024] = {0};
    char City02[1024] = {0};

    printf("City 01: ");
    fgets(City01, 1024, stdin);
    if (City01[strlen(City01)-1] == '\n') City01[strlen(City01)-1] = '\0';

    printf("City 02: ");
    fgets(City02, 1024, stdin);
    if (City02[strlen(City02)-1] == '\n') City02[strlen(City02)-1] = '\0';

    printf("Answer: \n");
}

void Functional_4()
{
    char StartTime[1024] = {0};
    char EndTime[1024] = {0};

    printf("Start time: ");
    fgets(StartTime, 1024, stdin);
    if (StartTime[strlen(StartTime)-1] == '\n') StartTime[strlen(StartTime)-1] = '\0';

    printf("End   time: ");
    fgets(EndTime, 1024, stdin);
    if (EndTime[strlen(EndTime)-1] == '\n') EndTime[strlen(EndTime)-1] = '\0';

    char City01[1024] = {0};

    printf("City 01: ");
    fgets(City01, 1024, stdin);
    if (City01[strlen(City01)-1] == '\n') City01[strlen(City01)-1] = '\0';

    printf("Answer: \n");
}

int main(void)
{
    char iBuffer[1024] = {0};

    printf("Please enter the file name: ");

    fgets(iBuffer, 1024, stdin);
    if (iBuffer[strlen(iBuffer)-1] == '\n') iBuffer[strlen(iBuffer)-1] = '\0';

    FILE *pFile = NULL;

    if((pFile = fopen(iBuffer, "rb")) == NULL)
    {
        printf("File could not be opened!\n");
        return 0;
    }

    int Choice = 0;

    sCSV csv;
    init(&csv, pFile);

    printf("Choice (1-4, 0: Exit): ");
    fgets(iBuffer, 1024, stdin);
    sscanf(iBuffer, "%d", &Choice);

    while (Choice != 0)
    {
        switch (Choice)
        {
            case 1:
                Functional_1(csv);
                break;
            case 2:
                Functional_2();
                break;
            case 3:
                Functional_3();
                break;
            case 4:
                Functional_4();
                break;
            default:
                break;
        }
        printf("Choice (1-4, 0: Exit): ");
        fgets(iBuffer, 1024, stdin);
        sscanf(iBuffer, "%d", &Choice);
    }
    fclose( pFile );

    printf("Bye.\n");

    return 0;
}
