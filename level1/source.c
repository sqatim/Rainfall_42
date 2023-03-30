#include <stdio.h>

int run()
{
    fwrite("Good... Wait what?\n", 1, 19, stdout);
    system("/bin/sh");
    return (0);
}

int main()
{
    char ptr[68];

    gets(ptr);
    return (0);
}