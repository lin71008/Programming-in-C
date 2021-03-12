#include <stdio.h>
#include <stdlib.h>

#define N 2048

int main()
{
    int Choice;
    char ibuffer[N], obuffer[N] = {0};

    printf("Please enter the JSON string:\n");
    fgets(ibuffer, N, stdin);

    printf("\nChoice (0:Exit,1:Get) : ");
    scanf("%d", &Choice);

    while(scanf("%d", &Choice) != EOF)
    {
        printf("Key: ");
        fgets(ibuffer, N, stdin);
        printf("Value: %s\n", obuffer);

        printf("Choice (0:Exit,1:Get) : ");
        scanf("%d", &Choice);
    }
    printf("Bye\n");

    return 0;
}
