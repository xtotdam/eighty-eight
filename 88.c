#include "stdio.h"
#include "stdlib.h"

int i, j;
int field[8][8] = {0};

int print()
{
    int state;
    int number;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            state = field[i][j] / 10;
            number = field[i][j] % 10;
            printf("%2d %2d", state, number);  //debug
            if ((state == 0) && (number > 0) && (number < 9))
            {
                printf("\e[01;38;05;%dm[%d]\e[0m", 20 * (field[i][j] % 10 + 3), field[i][j] % 10);
            }
            else if ((number == 9))
            {
                printf("\e[01;38;05;242m[@]\e[0m");
            }
            else if ((state == 1))
            {
                printf("\e[01;38;05;242m[%d]\e[0m", field[i][j] % 10);
            }
            else if ((state == 2))
            {
                printf("\e[01;38;05;242m[*]\e[0m");
            }
            else
            {
                printf("[ ]");
            }
        }
        printf("\n");
    }
    printf("\e[0;m");
    return 0;
}

int main(int argc, char const *argv[])
{
    for (i = 0; i < 8; i++)
    {
        field[4][i] = 9;
        field[5][i] = 21 + i;
        field[6][i] = 11 + i;
        field[7][i] = 1 + i;
    }
    print();
    return 0;
}