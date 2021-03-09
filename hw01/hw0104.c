#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 2048

static char buffer[100] = {0};
static char* tab(int n)
{
    for (int i = 0; i < n; ++i)
    {
        buffer[i] = ' ';
    }
    buffer[n] = '\0';
    return buffer;
}

#define DEFAULTSTAT 0
#define LISTSTAT 1
#define DICTSTAT 2
#define NODESTAT 3

static char strcvbuffer[N] = {0};
char* strconvert(const char* str, const int len)
{
    int idx = 0, idy = 0;
    char *p = strchr(str, '\\');
    while (p != NULL)
    {
        strncpy(strcvbuffer+idy, str+idx, p-str-idx);
        idy += p-str-idx;
        idx += p-str-idx;
        if (str[idx+1] == '0')
        {
            strcvbuffer[idy] = '\0';
        }
        else if (str[idx+1] == 't')
        {
            strcvbuffer[idy] = '\t';
        }
        else if (str[idx+1] == 'n')
        {
            strcvbuffer[idy] = '\n';
        }
        else if (str[idx+1] == '\\')
        {
            strcvbuffer[idy] = '\\';
        }
        else if (str[idx+1] == '\'')
        {
            strcvbuffer[idy] = '\'';
        }
        else if (str[idx+1] == '\"')
        {
            strcvbuffer[idy] = '\"';
        }
        idx += 2;
        idy += 1;
        p = strchr(p+2, '\\');
    }
    if (idx < len)
    {
        strncpy(strcvbuffer+idy, str+idx, len-idx);
        idy += len-idx;
    }
    strcvbuffer[idy] = '\0';
    return strcvbuffer;
}

void decode(char* str)
{
    int lv = 0, newnode = 1, iskey = 0, isvalue = 0;
    int stack[N] = {DEFAULTSTAT};
    while (*str != '\0')
    {
        if (strspn(str, " \n\t"))
        {
            printf("[Escape %d padding]", strspn(str, ", \n\t"));
            str += strspn(str, " \n\t");
            continue;
        }
        else if (*str == ':')
        {
            if (lv == 0 || stack[lv-1] != DICTSTAT || iskey == 1 || newnode == 1 || isvalue == 1)
            {
                printf("INV JSON\n");
                break;
            }
            printf("\n%s[Begin Node]", tab(lv-1));
            newnode = 1; isvalue = 1;
        }
        else if (*str == ',')
        {
            if (lv == 0 || newnode == 1 || (stack[lv-1] == DICTSTAT && 0))
            {
                printf("INV JSON\n");
                break;
            }
            printf("\n%s[End Node]", tab(lv-1));
            printf("\n%s[Begin Node]", tab(lv-1));
            newnode = 1;
            if (stack[lv-1] == DICTSTAT)
            {
                iskey = 1;
            }
        }
        else if (*str == '[')
        {
            if (lv == 0) printf("\n[Begin JSON]");
            if (newnode != 1 || iskey == 1)
            {
                printf("INV JSON\n");
                break;
            }
            printf("\n%s[Begin List]", tab(lv));
            printf("\n%s[Begin Node]", tab(lv));
            stack[lv++] = LISTSTAT;
            newnode = 1; isvalue = 0;
        }
        else if (*str == ']')
        {
            if (newnode == 1)
            {
               printf("\n%s[End Node]", tab(lv-1));
            }
            if (lv == 0 || stack[lv-1] != LISTSTAT)
            {
                printf("INV JSON\n");
                break;
            }
            printf("\n%s[End List]", tab(lv---1));
            newnode = 0; isvalue = 0;
            if (lv == 0)
            {
                printf("\n[End JSON]\n");
                break;
            }
        }
        else if (*str == '{')
        {
            if (lv == 0) printf("\n[Begin JSON]");
            if (newnode != 1 || iskey == 1)
            {
                printf("INV JSON\n");
                break;
            }
            printf("\n%s[Begin Dict]", tab(lv));
            printf("\n%s[Begin Node]", tab(lv));
            stack[lv++] = DICTSTAT;
            newnode = 1; iskey = 1; isvalue = 0;
        }
        else if (*str == '}')
        {
            if (lv == 0 || stack[lv-1] != DICTSTAT || isvalue == 1)
            {
                printf("INV JSON\n");
                break;
            }
            if (newnode == 1)
            {
               printf("\n%s[End Node]", tab(lv-1));
            }
            printf("\n%s[End Dict]", tab(lv---1));
            newnode = 0; isvalue = 0;
            if (lv == 0)
            {
                printf("\n[End JSON]\n");
                break;
            }
        }
        else if (strncmp(str, "true", 4) == 0)
        {
            if (lv == 0 || newnode != 1 || iskey == 1)
            {
                printf("INV JSON\n");
                break;
            }
            printf("\n%s[Boolean True]", tab(lv));
            newnode = 0; isvalue = 0;
            str += 4;
            continue;
        }
        else if (strncmp(str, "false", 4) == 0)
        {
            if (lv == 0 || newnode != 1 || iskey == 1)
            {
                printf("INV JSON\n");
                break;
            }
            printf("\n%s[Boolean False]", tab(lv));
            newnode = 0; isvalue = 0;
            str += 5;
            continue;
        }
        else if (strncmp(str, "null", 4) == 0)
        {
            if (lv == 0 || newnode != 1 || iskey == 1)
            {
                printf("INV JSON\n");
                break;
            }
            printf("\n%s[null]", tab(lv));
            newnode = 0; isvalue = 0;
            str += 4;
            continue;
        }
        else if (strspn(str, "-.0123456789eE"))
        {
            if (lv == 0 || newnode != 1 || iskey == 1)
            {
                printf("INV JSON\n");
                break;
            }
            char temp[N] = {0};
            strncpy(temp, str, strspn(str, "-.0123456789eE"));
            printf("\n%s[%lf]", tab(lv), atof(temp));
            newnode = 0; isvalue = 0;
            str += strspn(str, "-.0123456789eE");
            continue;
        }
        else if (*str == '\"')
        {
            if (lv == 0 || newnode != 1)
            {
                printf("INV JSON\n");
                break;
            }
            char* c = strchr(str+1, '\"');
            while (c != NULL)
            {
                *c = '\0';
                char *temp = c-1;
                while (temp > str && *temp == '\\')
                {
                    temp--;
                }
                if (c-temp-1 % 2 == 1)
                {
                    *c = '\"';
                    c = strchr(c+1, '\"');
                }
                else
                {
                    printf("\n%s[%s]", tab(lv), strconvert(str+1, c-str-1));
                    if (stack[lv-1] == DICTSTAT)
                    {
                        if (iskey == 1)
                        {
                            printf("[Is Key]");
                            iskey = 0;
                            isvalue = 1;
                        }
                    }
                    printf("\n%s[End Node]", tab(lv-1));
                    newnode = 0; isvalue = 0;
                    str = c;
                    break;
                }
            }
            if (c == NULL)
            {
                printf("INV JSON\n");
                break;
            }
        }
        else
        {
            printf("INV JSON\n");
            break;
        }
        str++;
    }
}

int main(int argc, char const *argv[])
{
    int Choice;
    char ibuffer[N], obuffer[N] = {0}, jbuffer[2*N] = {0};

    // printf("Please enter the JSON string:\n");
    while (fgets(ibuffer, N, stdin) != NULL)
    {
        strcat(jbuffer, ibuffer);
    }
    decode(jbuffer);

    // printf("\nChoice (0:Exit,1:Get) : ");
    // scanf("%d", &Choice);

    // while(scanf("%d", &Choice) != EOF)
    // {
    //     // printf("Key: ");
    //     fgets(ibuffer, N, stdin);
    //     // printf("Value: %s\n", obuffer);

    //     // printf("Choice (0:Exit,1:Get) : ");
    //     scanf("%d", &Choice);
    // }
    // printf("Bye\n");

    return 0;
}
