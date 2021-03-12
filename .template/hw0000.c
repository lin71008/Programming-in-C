#include <stdio.h>

int main()
{
    char buffer[20];
    fgets(buffer, sizeof(buffer), stdin);
    printf("%s", buffer);
    return 0;
}
