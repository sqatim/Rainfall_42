#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void p(char *str, char *dash)
{
    char *occurrence;
    char ptr1[4096];
    puts(dash);
    read(0, ptr1, 4096);
    occurrence = strchr(ptr1, '\n');
    *occurrence = '\0';
    strncpy(str, ptr1, 20);
}

void pp(char *str)
{
    char ptr1[20];
    char ptr2[20];
    p(ptr1, " - ");
    p(ptr2, " - ");
    strcpy(str, ptr1);
    str[strlen(str)] = ' ';
    strcat(str, ptr2);
}

int main()
{
    char str[166];
    pp(str);
    puts(str);
    return (0);
}
