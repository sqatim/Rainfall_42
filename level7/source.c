#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char c[68];

typedef struct s_data
{
    int number;
    char *ptr;
} t_data;

void m()
{
    int var;

    var = time(NULL);
    printf("%s - %d\n", c, var);
}

int main(int ac, char **av)
{
    FILE *file;
    t_data *data1;
    data1 = malloc(8);
    data1->number = 1;
    char *ptr1 =  malloc(8);
    data1->ptr = ptr1;

    t_data *data2;
    data2 = malloc(8);
    data2->number = 2;
    char *ptr2 =  malloc(8);
    data2->ptr = ptr2;

    strcpy(data1->ptr, av[1]);
    strcpy(data1->ptr, av[2]);
    file = fopen("/home/user/level8/.pass", "r");
    fgets(c, 68, file);
    puts("~~");
    return 0;
}
