#include <stdio.h>

void p(char *ptr)
{
    printf(ptr);
}

void n()
{
    char ptr[513];
    fgets(ptr, 512, stdin);
    p(ptr);
    system();
}

int main()
{
    n();
    return (0);
}