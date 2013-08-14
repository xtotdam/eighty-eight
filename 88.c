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
            if (field[i][j] == 19)
            {
                field[i][j] = 9;    //because no bombs are greyed
            }

            state = field[i][j] / 10;
            number = field[i][j] % 10;
            printf("%2d %2d", state, number);   //debug
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
            printf("%d ", fall[i]);   //falling debug
            fall[i] = 0;
        }
        printf(": %d \n", fallen);    //falling debug
    }
    while (fallen != 0);
    return 0;
}

int init()
{
    int seed = 1;
    printf("Enter integer seed for random generator :   ");
    scanf("%d", &seed);
    printf("\n\n");
    srand(seed);
    for (i = 0; i < 16; i++)
    {
        field[rand() % 8][rand() % 8] = 20 * (rand() % 2) + rand() % 9 + 1;
    }
    print();
    fall();
    return 0;
}

int main(int argc, char const *argv[])
{
    init();
    print();


    return 0;
}