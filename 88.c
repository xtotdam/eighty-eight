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

int fall()
{
    int fall[8] = {0};
    int fallen = 0;
    int temp = 0;
    do
    {
        fallen = 0;
        for (j = 0; j < 8; j++)
        {
            for (i = 0; i < 8 - 1; i++)
            {
                if ((field[i + 1][j] % 10 == 0) && (field[i][j] % 10 != 0))
                {
                    temp = field[i + 1][j];
                    field[i + 1][j] = field[i][j];
                    field[i][j] = temp;

                    fall[j] = 1;
                }
            }
        }
        for (i = 0; i < 8; i++)
        {
            fallen += fall[i];
            printf("%d ", fall[i]);
            fall[i] = 0;
        }
        printf(": %d \n", fallen);
    }
    while (fallen != 0);
    return 0;
}

int main(int argc, char const *argv[])
{
    // for (i = 0; i < 8; i++)
    // {
    //     for (j = 0; j < 8; j++)
    //     {
    //         field[i][j] = 10 * i + j;
    //     }
    // }
    // print();
    // printf("\n\n");

    for (i = 0; i < 8; i++)
    {
        // field[4][i] = 9;
        // field[5][i] = 21 + i;
        // field[6][i] = 11 + i;
        //field[7][i] = 1 + i;
    }
    field[1][1] = 11;
    field[3][1] = 12;
    field[4][2] = 13;
    field[0][4] = 14;
    field[2][4] = 14;
    field[4][4] = 14;
    field[6][4] = 14;
    print();
    fall();
    print();


    return 0;
}