#include "stdio.h"
#include "stdlib.h"

int main(int argc, char const *argv[])
{
    srand(15468451);
    int probab[31] = {0};
    int total = 1000000;
    int next;
    for (int i = 0; i < total; i++)
    {
        next = 20 * (rand() % 2) + rand() % 9 + 1;
        probab[next]++;
    }
    printf("total: %d\n", total);
    for (int i = 0; i < 31; i++)
    {
        printf("%d : %d : %.3lf%%\n", i, probab[i], (double)probab[i] / total * 100);
    }
    return 0;
}