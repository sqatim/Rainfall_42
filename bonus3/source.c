#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av)
{
    char str[132];
    int number;
    FILE *file;

    file = fopen("/home/user/end/.pass", "r");
    bzero(str, 132);
    if (file == 0 || ac != 2)
        return (-1);
    fread(str, 1, 66, file);
    str[65] = 0;
    number = atoi(av[1]);
    str[number] = 0;
    fread(str + 66, 1, 65, file);
    fclose(file);
    if (strcmp(str, av[1]) == 0)
    {
        execl("/bin/sh", "ss", NULL);
        return (0);
    }
    puts(str + 66);
    return (0);
}