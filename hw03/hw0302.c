#include <stdio.h>

void menu();

int main()
{
    printf("Please open a season record: ");
    menu();
    printf("Choice (1-7, 8:exit): ");
    printf("Bye.\n");
    return 0;
}

void menu()
{
    printf("1) Who is the winner in this season?");
    printf("2) Which team gets the most scores?");
    printf("3) Which team gets the most red cards?");
    printf("4) Which team wins the most games at home?");
    printf("5) Which team wins the most games away from home?");
    printf("6) Which game has the most scoring gap?");
    printf("7) Team information.");
    printf("8) Exit");
}
