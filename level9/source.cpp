#include <iostream>
#include <cstring>

class N
{
private:
    int number;
    char str[100];

public:
    N(int i): number(i)
    {
    }
    void setAnnotation(char *ptr)
    {
        int len = strlen(ptr);
        memcpy(str, ptr, len);
    }
    int operator+(const N &other)
    {
        return this->number + other.number;
    }
};

int main(int ac, char **av)
{
    int number;
    if (ac < 2)
        exit(1);

    N *first = new N(5);
    N *second = new N(6);

    first->setAnnotation(av[1]);
    number = second->operator+(*first);
    return (number);
}