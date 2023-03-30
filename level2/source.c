#include <stdio.h>

int p()
{
    char ptr[76];
    unsigned int returnAddress;

    fflush(stdout);
    gets(ptr);
    returnAddress = __builtin_return_address(0);
    if ((returnAddress & 0xb0000000) == 0xb0000000)
    {
        printf("(%p)\n", returnAddress);
        exit(1);
    }
    puts(ptr);
    strdup(ptr);
}

int main()
{
    p();
    return (0);
}