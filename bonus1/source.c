#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int ac, char **av)
{
    int number;
    char str[36];

    number = atoi(av[1]);
    if (number > 9)
        return (1);
    memcpy(str, av[2], number * 4);
    if (number != 1464814662)
        return (0);
    execl("/bin/sh", "sh", NULL);
    return (0);
}