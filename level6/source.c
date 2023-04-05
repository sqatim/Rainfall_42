#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void m()
{
    puts("Nope");
}

void n()
{
    system("/bin/cat /home/user/level7/.pass");
}

int main(int ac, char **av)
{
    void *ptr1;

    void (*function)();
    ptr1 = malloc(64);
    function = malloc(4);
    function = &m;
    strcpy(ptr1, av[1]);
    function();

    return (0);
}