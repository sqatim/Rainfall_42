#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int language;

void greetuser(char *str)
{
    char ptr[84];
    if (language == 0)
    {
        ptr[0] = 'H';
        ptr[1] = 'e';
        ptr[2] = 'l';
        ptr[3] = 'l';
        ptr[4] = 'o';
        ptr[5] = ' ';
        ptr[6] = '\0';
    }
    if (language == 1)
    {
        ptr[0] = 0x48;
        ptr[1] = 0x79;
        ptr[2] = 0x76;
        ptr[3] = 0xc3;
        ptr[4] = 0xa4;
        ptr[5] = 0xc3;
        ptr[6] = 0xa4;
        ptr[7] = 0x20;
        ptr[8] = 0x70;
        ptr[9] = 0xc3;
        ptr[10] = 0xa4;
        ptr[11] = 0x69;
        ptr[12] = 0x76;
        ptr[13] = 0xc3;
        ptr[14] = 0xa4;
        ptr[15] = 0xc3;
        ptr[16] = 0xa4;
        ptr[17] = 0x20;
        ptr[18] = 0x00;
    }
    else if (language == 2)
    {
        
        ptr[0] = 0x47;
        ptr[1] = 0x6f;
        ptr[2] = 0x65;
        ptr[3] = 0x64;
        ptr[4] = 0x65;
        ptr[5] = 0x6d;
        ptr[6] = 0x69;
        ptr[7] = 0x64;
        ptr[8] = 0x64;
        ptr[9] = 0x61;
        ptr[10] = 0x67;
        ptr[11] = 0x21;
        ptr[12] = 0x20;
        ptr[13] = 0x00;
    }
    strcat(ptr, str);
    puts(ptr);
}

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
    if (!env)
        greetuser(str);
    else
    {
        if (!memcmp(env, "fi", 2))
            language = 1;
        else if (!memcmp(env, "nl", 2))
            language = 2;
        greetuser(str);
    }
    return (0);
}