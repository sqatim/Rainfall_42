#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *service;
char *auth;

int main()
{
    char ptr[128];

    while (1)
    {

        printf("%p, %p \n", auth, service);
        fgets(ptr, 128, stdin);
        if (!ptr)
            break;
        if (!strncmp(ptr, "auth ", 5))
        {
            auth = malloc(4);
            auth[0] = 0;
            if (strlen(ptr + 5) <= 30)
                strcpy(auth, ptr + 5);
        }
        if (!strncmp(ptr, "reset", 5))
            free(auth);
        if (!strncmp(ptr, "service", 6))
            service = strdup(ptr + 7);
        if (!strncmp(ptr, "login", 5))
        {
            if (*(auth + 32))
                system("/bin/sh");
            else
                fwrite("Password:\n", 1, 10, stdout);
        }
    }
}