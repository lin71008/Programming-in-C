#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

sJSON* StringToJSON(char *str)
{
    if (str == NULL) return NULL;

    if (*str != '{') return NULL;  // JSON root must be an object.

    sJSON *root = (sJSON*) malloc(sizeof(sJSON));
    root->type = Object;
    root->key = NULL;
    root->value = NULL;
    root->next = NULL;
    str++;

    int top = 1;
    sJSON *stack[100];
    stack[0] = root;

    while (*str != '\0')
    {
        if (*str == '{')
        {
            if (stack[top-1]->value != NULL)
            {
                sJSON *temp = (sJSON*) malloc(sizeof(sJSON));
                temp->type = stack[top-1]->type;
                temp->key = NULL;
                temp->value = NULL;
                temp->next = NULL;
                stack[top-1]->next = temp;
                stack[top-1] = stack[top-1]->next;
            }
            sJSON *temp = (sJSON*) malloc(sizeof(sJSON));
            temp->type = Object;
            temp->key = NULL;
            temp->value = NULL;
            temp->next = NULL;
            stack[top-1]->value = temp;
            stack[top++] = temp;
        }
        else if (*str == '}')
        {
            if (top == 1)
            {
                return root;
            }
            else
            {
                top--;
            }
        }
        else if (*str == '[')
        {
            if (stack[top-1]->value != NULL)
            {
                sJSON *temp = (sJSON*) malloc(sizeof(sJSON));
                temp->type = stack[top-1]->type;
                temp->key = NULL;
                temp->value = NULL;
                temp->next = NULL;
                stack[top-1]->next = temp;
                stack[top-1] = stack[top-1]->next;
            }
            sJSON *temp = (sJSON*) malloc(sizeof(sJSON));
            temp->type = Array;
            temp->key = NULL;
            temp->value = NULL;
            temp->next = NULL;
            stack[top-1]->value = temp;
            stack[top++] = temp;
        }
        else if (*str == ']')
        {
            top--;
        }
        else if (*str != ',' && *str != ':')
        {
            int len = strcspn(str, ",:}]");
            if (*str == '\"')
            {
                str++;
                len--;
            }
            char *str1 = malloc(sizeof(char)*(len+1));
            strncpy(str1, str, len);
            str1[len] = '\0';
            if (str1[len-1] == '\"') str1[len-1] = '\0';
            if (stack[top-1]->type == Array)
            {
                if (stack[top-1]->value != NULL)
                {
                    sJSON *temp = (sJSON*) malloc(sizeof(sJSON));
                    temp->type = Array;
                    temp->key = NULL;
                    temp->value = NULL;
                    temp->next = NULL;
                    stack[top-1]->next = temp;
                    stack[top-1] = temp;
                }
                sJSON *temp = (sJSON*) malloc(sizeof(sJSON));
                temp->type = String;
                temp->key = str1;
                temp->value = NULL;
                temp->next = NULL;
                stack[top-1]->value = temp;
            }
            else
            {
                if (stack[top-1]->value != NULL)
                {
                    sJSON *temp = (sJSON*) malloc(sizeof(sJSON));
                    temp->type = Object;
                    temp->key = NULL;
                    temp->value = NULL;
                    temp->next = NULL;
                    stack[top-1]->next = temp;
                    stack[top-1] = temp;
                }
                if (stack[top-1]->key == NULL)
                {
                    stack[top-1]->key = str1;
                }
                else
                {
                    sJSON *temp = (sJSON*) malloc(sizeof(sJSON));
                    temp->type = String;
                    temp->key = str1;
                    temp->value = NULL;
                    temp->next = NULL;
                    stack[top-1]->value = temp;
                }
            }
            str += len;
            continue;
        }
        str++;
    }
    return NULL;
}

sJSON* JSONGetValue(const sJSON *source, const char *key)
{
    if (key == NULL) return NULL;
    if (source == NULL || source->type != Object) return NULL;
    while(source != NULL && source->key != NULL)
    {
        if (strcmp(source->key, key) == 0)
        {
            return source->value;
        }
        source = source->next;
    }
    return NULL;
}

sJSON* JSONGetItem(const sJSON *source, int index)
{
    if (source == NULL || source->type != Array) return NULL;
    while(source != NULL)
    {
        if (index == 0)
        {
            return source->value;
        }
        index--;
        source = source->next;
    }
    return NULL;
}

void PrintJSON(const sJSON *source)
{
    if (source == NULL) return;
    if (source->type == String && source->key != NULL)
    {
        printf("%s", source->key);
    }
    else if (source->type == Array)
    {
        printf("[");
        while (source != NULL)
        {
            PrintJSON(source->value);
            source = source->next;
            if (source != NULL)printf(",");
        }
        printf("]");
    }
    else
    {
        printf("{");
        while (source != NULL)
        {
            printf("%s:", source->key);
            PrintJSON(source->value);
            source = source->next;
            if (source != NULL)printf(",");
        }
        printf("}");
    }
}
