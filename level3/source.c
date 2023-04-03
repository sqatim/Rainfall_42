#include <stdio.h>

void v()
{
    int *number;
    char ptr[512];
    fgets(ptr, 512, stdin);
    printf(ptr);
    number = 0x804988c;
    if (*number == 64)
    {
        fwrite("Wait what?!\n", 1, 12, stdout);
        system("/bin/sh");
    }
}

int main()
{
    v();
    return (0);
}