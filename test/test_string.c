#include "stdio.h"
#include "string.h"

int main(int argc, char const *argv[])
{
    char arg[] = "--seed=123"; //strlen=10
    char str[] = "abcdefghij";

    printf("%s %c\n", str, str[5]);
    str[5] = 'q';
    printf("%s %c\n", str, str[5]);

    return 0;
}