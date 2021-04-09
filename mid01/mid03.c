#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


int main(void)
{
    char iBuffer[1024] = {0};
    char StartTime[1024] = {0};
    char EndTime[1024] = {0};

    printf("Please enter the file name: ");

    fgets(iBuffer, 1024, stdin);
    if (iBuffer[strlen(iBuffer)-1] == '\n') iBuffer[strlen(iBuffer)-1] = '\0';

    FILE    *pFile = NULL;

    if( ( pFile = fopen( iBuffer, "rb" ) ) == NULL )
    {
        printf( "File could not be opened!\n" );
        return 0;
    }

    int Choice = 0;

    printf("Choice (1-4, 0: Exit): ");
    scanf("%d", &Choice);

    while (Choice != 0)
    {
        printf("Start time: ");

        fgets(StartTime, 1024, stdin);
        if (StartTime[strlen(StartTime)-1] == '\n') StartTime[strlen(StartTime)-1] = '\0';

        printf("End   time:");

        fgets(EndTime, 1024, stdin);
        if (EndTime[strlen(EndTime)-1] == '\n') EndTime[strlen(EndTime)-1] = '\0';

        if (0)
        {
            printf("Answer: %s\n", "");
        }
        else
        {
            for (int i = 0; i < 3; ++i)
            {
                printf("City%02d: ", i);
            }
        }

        printf("Choice (1-4, 0: Exit): ");
        scanf("%d", &Choice);
    }
    fclose( pFile );

    printf("Bye.\n");

    return 0;
}
