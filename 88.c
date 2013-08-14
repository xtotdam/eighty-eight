#include "stdio.h"
#include "stdlib.h"

int i, j;
int field[8][8] = {0};
int score = 0;
int next = 0;
int gameover = 0;

int print()
{
    int state;
    int number;
    for (i = 0; i < 8; i++)
    {
        printf(" %d | ", 8 - i);
        for (j = 0; j < 8; j++)
        {
            if (field[i][j] == 19)
            {
                field[i][j] = 9;    //because no bombs can be greyed
            }

            state = field[i][j] / 10;
            number = field[i][j] % 10;
            printf("%2d %2d", state, number);   //debug
            if ((state == 0) && (number > 0) && (number < 9))
            {
                printf("\e[01;38;05;%dm[%d]\e[0m", 20 * (number + 3), number);
            }
            else if ((number == 9))
            {
                printf("\e[01;38;05;242m[@]\e[0m");
            }
            else if ((state == 1))
            {
                printf("\e[01;38;05;242m[%d]\e[0m", number);
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
        if (i == 1)
        {
            printf("\tNext : %d", next);
        }
        else if (i == 4)
        {
            printf("\tScore : %d", score);
        }
        printf("\n");
    }
    printf("   |\n");
    printf("    _________________________________________________________________\n");
    printf("           1       2       3       4       5       6       7       8 \n");
    // printf("    ________________________\n");
    // printf("     1  2  3  4  5  6  7  8 \n");    //for final visualisation
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
    //print();  //debug
    fall();
    return 0;
}

int main(int argc, char const *argv[])
{
    int place = 0;

    init();
    while (gameover != 1)
    {
        next = 20 * (rand() % 2) + rand() % 9 + 1;
        print();
        do
        {
            scanf("%d", &place);
        }
        while ((place < 0) && (place > 9));
        field[0][place - 1] = next;
        fall();

    }
    return 0;
}