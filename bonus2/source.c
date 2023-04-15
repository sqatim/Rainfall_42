// #include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int ac, char **av)
{
    char str[76];
    char *env;

    if (ac != 3)
        return (1);
    bzero(str, 76);
    strncpy(str, av[1], 40);
    strncpy(str + 40, av[2], 32);
    env = getenv("LANG");
    if(!env)
    {
        // greet
    }
    return (0);
}