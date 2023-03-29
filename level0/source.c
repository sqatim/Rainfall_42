#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int ac, char **av)
{
    char *ptr;
    int group;
    int user;
    char *args[] = {"/bin/sh", NULL};
    
    if (atoi(av[1]) == 423)
    {
        ptr = strdup("/bin/sh");
        group = getegid();
        user = geteuid();
        setresgid(group, group, group);
        setresuid(user, user, user);
        execv(ptr, args);
    }
    else
        fwrite("No !\n", 0x1, 0x5, stderr);
    return (0);
}