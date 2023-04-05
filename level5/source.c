#include <stdio.h>

void o()
{
    system("/bin/sh");
    exit(1);
}

void n()
{
    char ptr[512];
    fgets(ptr, 512, stdin);
    printf(ptr);
    exit(1);
}

int main()
{
    n();
    return (0);
}