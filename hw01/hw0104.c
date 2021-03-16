#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

#define N 2048

int main()
{
    int Choice;
    char ibuffer[N], key[N];

    printf("Please enter the JSON string:\n");
    fgets(ibuffer, N, stdin);

    if (ibuffer[strlen(ibuffer)-1] == '\n') ibuffer[strlen(ibuffer)-1] = '\0';  // remove '\n'

    sJSON *json = StringToJSON(ibuffer);

    printf("\nChoice (0:Exit,1:Get) : ");
    scanf("%d", &Choice);

    while(scanf("%d", &Choice) != EOF)
    {
        printf("Key: ");
        fgets(key, N, stdin);

        if (key[strlen(key)-1] == '\n') key[strlen(key)-1] = '\0';  // remove '\n'

        sJSON *value = json;
        char *k = strtok(key, ".");

        while (k != NULL)
        {
            char *l = strchr(k, '[');
            if (l != NULL)
            {
                *l = '\0';
                l++;
                int idx = 0;
                while (*l != ']')
                {
                    idx = idx * 10 + (int)(*l - '0');
                    l++;
                }
                value = JSONGetValue(value, k);
                value = JSONGetItem(value, idx);
            }
            else
            {
                value = JSONGetValue(value, k);
            }
            k = strtok(NULL, ".");
        }
        if (value != NULL)
        {
            printf("Value: ");
            PrintJSON(value);
            printf("\n");
        }
        else
        {
            printf("Given Key does not exist in JSON\n");
        }

        printf("Choice (0:Exit,1:Get) : ");
        scanf("%d", &Choice);
    }
    printf("Bye\n");

    return 0;
}
