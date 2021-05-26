/*************************************************************
Code obfuscation...
Level 1: random padding
Level 2: random variable name
Level 3: random function name
Level 4: unreadable integer constant
level ?: add random and nonsense comment
Level ?: using ISO646, digraphs, trigraphs
Level ?: abuse typedef ?
*************************************************************/

// single line comment

/*
multi-line  comment
// comment in comment
*/

/* code in comment
int max (int a, int b)
{
    return a < b ? a : b;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// single line macro
#define HELLO "hello"

// multi-line macro
#define MAX(X, Y) \
((X) > (Y) ? (X) : (Y))

// simple function
void hello(void)
{
    printf("%s\n", HELLO);
}

// recursive function
int fib(int n)
{
    return n < 2 ? 1 : fib(n-1) + fib(n-2);
}

// main function
int main()
{
    // variable
    int a = 8, b = 32;
    char pi[] = "3.1415...";
    float e = 2.718281;
    int n = 10;

    printf("counting... ");
    for (int i = 0; i < n; ++i)
    {
        printf("%d%c", i, " \n"[i == n-1]);
    }

    printf("a = %d, b = %d, 8 + 32 = %d, max(a,b) = %d\n", \
        a, b, a + b, MAX(a, b));

    printf("pi = %s, e = %f...\n", pi, e);

    printf("Fibonacci sequence: ");
    for (int i = 0; i < n; ++i) { printf("%d%c", fib(i), "\n "[i != n-1]); }

    return 0;
}
