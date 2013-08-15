#include "stdio.h"
#include "stdlib.h"

int i, j;
int field[8][8] = {0};
int horb[8][8] = {0};
int verb[8][8] = {0};
int score = 0;
int next = 0;
int gameover = 0;
int state, number;
int tobechecked = 0;

int print()
{
    for (i = 0; i < 8; i++)
    {
        printf(" %d | ", i);
        for (j = 0; j < 8; j++)
        {
            if (field[i][j] == 19)
            {
                field[i][j] = 9;    //because no bombs can be grey
            }

            state = field[i][j] / 10;
            number = field[i][j] % 10;
            printf("%2d %2d", state, number);   //debug

            if      ((state == 1))  printf("\e[01;38;05;242m[%d]\e[0m", number);
            else if ((state == 2))  printf("\e[01;38;05;242m[*]\e[0m");
            else if ((state == 0))
            {
                switch (number)
                {
                case 0: printf("[ ]"); break;
                case 1: printf("\e[01;38;05;80m[1]\e[0m"); break;
                case 2: printf("\e[01;38;05;100m[2]\e[0m"); break;
                case 3: printf("\e[01;38;05;120m[3]\e[0m"); break;
                case 4: printf("\e[01;38;05;140m[4]\e[0m"); break;
                case 5: printf("\e[01;38;05;160m[5]\e[0m"); break;
                case 6: printf("\e[01;38;05;180m[6]\e[0m"); break;
                case 7: printf("\e[01;38;05;200m[7]\e[0m"); break;
                case 8: printf("\e[01;38;05;220m[8]\e[0m"); break;
                case 9: printf("\e[01;38;05;240m[@]\e[0m"); break;  //bomb
                default: printf("XXX"); break;
                }
            }
        }
        if (i == 1)
        {
            printf("\tNext : ");
            state = next / 10;
            number = next % 10;
            printf("%2d %2d", state, number);   //debug

            if      ((state == 2))  printf("\e[01;38;05;242m[*]\e[0m");
            else if ((state == 0))
            {
                switch (number)
                {
                case 1: printf("\e[01;38;05;80m[1]\e[0m"); break;
                case 2: printf("\e[01;38;05;100m[2]\e[0m"); break;
                case 3: printf("\e[01;38;05;120m[3]\e[0m"); break;
                case 4: printf("\e[01;38;05;140m[4]\e[0m"); break;
                case 5: printf("\e[01;38;05;160m[5]\e[0m"); break;
                case 6: printf("\e[01;38;05;180m[6]\e[0m"); break;
                case 7: printf("\e[01;38;05;200m[7]\e[0m"); break;
                case 8: printf("\e[01;38;05;220m[8]\e[0m"); break;
                case 9: printf("\e[01;38;05;240m[@]\e[0m"); break;  //bomb
                default: printf("XXX"); break;
                }
            }
        }
        else if (i == 4)
        {
            printf("\tScore : %d", score);
        }
        printf("\n");
    }
    printf("   |\n");
    printf("    _________________________________________________________________\n");
    printf("           0       1       2       3       4       5       6       7 \n");
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
        printf("Falling...  ");
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
    // printf("Enter integer seed for random generator :   ");
    // scanf("%d", &seed);
    // printf("\n\n");
    srand(seed);
    for (i = 0; i < 20; i++)
    {
        field[rand() % 8][rand() % 8] = 20 * (rand() % 2) + rand() % 9 + 1;
    }
    //print();  //debug
    fall();
    return 0;
}

int destroy();
int check()
{
    int up, right, down, left;
    int hor, ver;
    int k;

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            state  = field[i][j] / 10;
            number = field[i][j] % 10;
            up = 0; right = 0; down = 0; left = 0;

            if ((field[i][j] != 0) && (state == 0) && (number != 0) && (number != 9))
            {
                up = 0; right = 0; down = 0; left = 0;
                printf("Checking... %d%d [%d] ", i, j, number);
                for (k = i - 1; k > (-1); k--)
                {
                    if (field[k][j] == 0) break;
                    up++;
                }
                for (k = j + 1; k < 8; k++)
                {
                    if (field[i][k] == 0) break;
                    right++;
                }
                for (k = i + 1; k < 8; k++)
                {
                    if (field[k][j] == 0) break;
                    down++;
                }
                for (k = j - 1; k > (-1); k--)
                {
                    if (field[i][k] == 0) break;
                    left++;
                }
                printf(": %du %dr %dd %dl ", up, right, down, left);
                horb[i][j] = right + left + 1;
                verb[i][j] = up + down + 1;
                printf("| %dh %dv\n", horb[i][j], verb[i][j]);
            }
        }
    }
    destroy();
    return 0;
}

int destroy()
{
    int fnc = 0;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            state  = field[i][j] / 10;
            number = field[i][j] % 10;
            if ((field[i][j] != 0) && (state == 0) && (number != 0) && (number != 9))
            {
                if ((field[i][j] == horb[i][j]) || (field[i][j] == verb[i][j]))
                {
                    printf("destroy... %d %d [%d] %dh %dv\n", i, j, field[i][j], horb[i][j], verb[i][j]);
                    field[i][j] = 0;
                    fnc = 1;
                }
            }
        }
    }
    if (fnc == 1)
    {
        fall();
        check();
    }
}

int main()
{
    int place = 0;

    init();
    while (gameover != 1)
    {
        next = 20 * (rand() % 2) + rand() % 9 + 1;
        if (next == 19) next = 9;
        print();
        do
        {
            scanf("%d", &place);
        }
        while ((place < 0) && (place > 9));
        field[0][place - 1] = next;
        fall();
        check();
    }
    return 0;
}