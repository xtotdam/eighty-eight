#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define DEBUG 1
#define LOG 0   //does nothing

int i, j;
int field[8][8] = {0};
int horb[8][8] = {0};
int verb[8][8] = {0};
int score = 0;
int next = 0;
int gameover = 0;
int state, number;
int tobechecked = 0;
int fnc = 0;

int print();
int fall();
int init();
int check();
int destroy();
int startscreen();
int placenew();

int main()
{
    //TODO: logging as console argument?

    startscreen();
    init();
    while (gameover != 1)
    {
        placenew();
        check();
    }
    return 0;
}

int placenew()
{
    int place = 0;

    next = 20 * (rand() % 2) + rand() % 9 + 1;
    if (next == 19) next = 9;
    print();
    do
    {
        printf(" > ");
        scanf("%d", &place);
    }
    while ((place < 0) && (place > 9));
    field[0][place - 1] = next;
    if (DEBUG) printf("\e[01;38;05;110mA\e[0mdding...   [%d] > %d\n", next, place);
}

int print() //doesn't call any other functions
{
    for (i = 0; i < 8; i++)
    {
        if (DEBUG) printf(" %d | ", i);
        if (!DEBUG) printf(" %d | ", 8 - i);
        for (j = 0; j < 8; j++)
        {
            state = field[i][j] / 10;
            number = field[i][j] % 10;
            if (DEBUG) printf("%2d %2d", state, number);

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
            if (DEBUG) printf("%2d %2d", state, number);

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
        else if (i == 4) printf("\tScore : %d", score);
        printf("\n");
    }
    if (DEBUG)  printf("   |_________________________________________________________________\n");
    if (DEBUG)  printf("           0       1       2       3       4       5       6       7 \n");
    if (!DEBUG) printf("   |_________________________\n");
    if (!DEBUG) printf("      1  2  3  4  5  6  7  8 \n");
    printf("\e[0;m");
    return 0;
}

int fall()  //doesn't call any other functions
{
    int fall[8] = {0};
    int fallen = 0;
    int temp = 0;
    do
    {
        if (DEBUG) printf("\e[01;38;05;107mF\e[0malling...  ");
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
            if (DEBUG) printf("%d ", fall[i]);   //falling debug
            fall[i] = 0;
        }
        if (DEBUG) printf(": %d \n", fallen);    //falling debug
    }
    while (fallen != 0);
    return 0;
}

int init()  //calls fall(),print(),check()
{
    int seed = 1;
    //TODO: seed as console argument
    seed = time(NULL);
    srand(seed);
    printf("seed:  %d\n", seed);

    for (i = 0; i < 20; i++)
    {
        field[rand() % 8][rand() % 8] = 20 * (rand() % 2) + rand() % 9 + 1;
    }
    fall();
    print();
    check();
    return 0;
}

int check() //calls fall(),print(),destroy()
{
    int up, right, down, left;
    int hor, ver;
    int k;

    fall();
    //if (fnc == 1) print();    //it is useful to show chains, but is buggy now
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (field[i][j] == 19)
            {
                field[i][j] = 9;    //because no bombs can be grey
                printf("\e[01;38;05;222mI\e[0mgnite...  %d%d : (29 >) 19 > 9\n", i, j);
            }

            state  = field[i][j] / 10;
            number = field[i][j] % 10;
            up = 0; right = 0; down = 0; left = 0;

            if ((field[i][j] != 0) && (state == 0) && (number != 0) && (number != 9))
            {
                up = 0; right = 0; down = 0; left = 0;
                if (DEBUG) printf("\e[01;38;05;222mC\e[0mhecking... %d%d [%d] ", i, j, number);
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
                if (DEBUG) printf(": %du %dr %dd %dl ", up, right, down, left);
                horb[i][j] = right + left + 1;
                verb[i][j] = up + down + 1;
                if (DEBUG) printf("| %dh %dv\n", horb[i][j], verb[i][j]);
            }
        }
    }
    destroy();
    return 0;
}

int destroy()   //calls check() if destroys a block
{
    fnc = 0;
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
                    if (DEBUG) printf("\e[01;38;05;196mD\e[0mestroy...  %d %d [%d] %dh %dv\n", i, j, field[i][j], horb[i][j], verb[i][j]);
                    field[i][j] = 0;
                    fnc = 1;

                    if (((i - 1) >= 0) && ((i - 1) < 8) && (field[i - 1][j] > 9))
                    {
                        field[i - 1][j] -= 10;
                        printf("\e[01;38;05;196mR\e[0meveal...   %d%d : %d > %d\n", i - 1, j, field[i - 1][j] + 10, field[i - 1][j]);
                    }
                    if (((j + 1) >= 0) && ((j + 1) < 8) && (field[i][j + 1] > 9))
                    {
                        field[i][j + 1] -= 10;
                        printf("\e[01;38;05;196mR\e[0meveal...   %d%d : %d > %d\n", i, j + 1, field[i][j + 1] + 10, field[i][j + 1]);

                    }
                    if (((i + 1) >= 0) && ((i + 1) < 8) && (field[i + 1][j] > 9))
                    {
                        field[i + 1][j] -= 10;
                        printf("\e[01;38;05;196mR\e[0meveal...   %d%d : %d > %d\n", i + 1, j, field[i + 1][j] + 10, field[i + 1][j]);

                    }
                    if (((j - 1) >= 0) && ((j - 1) < 8) && (field[i][j - 1] > 9))
                    {
                        field[i][j - 1] -= 10;
                        printf("\e[01;38;05;196mR\e[0meveal...   %d%d : %d > %d\n", i , j - 1, field[i][j - 1] + 10, field[i ][j - 1]);

                    }
                }
            }
        }
    }
    if (fnc == 1) check();
    return 0;
}

int startscreen()   //doesn't call any other functions
{
    if (DEBUG) printf("*2-4-6-8*1*2-4-6-8*2*2-4-6-8*3*2-4-6-8*4*2-4-6-8*5*2-4-6-8*6*2-4-6-8*7*2-4-6-8*8*2-4-6-8*9\n\n");
    printf("  **                                                                                 **   \n");
    printf("  *                                   Eighty  Eight                                   *   \n");
    printf("  **                                                                                 **   \n");
    printf("\n\n");
    printf("  Rules:\n");
    printf("\e[01;38;05;107m     v  Block disappears, if its number equals amount of blocks in the same row/column.\n");
    printf("\e[01;38;05;107m     v  Gray blocks are uncovered by breaking touching blocks.\n");
    printf("\e[01;38;05;242m     x  Bomb can be triggered by breaking block in the same row/column.\n");
    printf("\e[01;38;05;107m     v  When block disappears, block above will drop.\n");
    printf("\e[01;38;05;242m     x  New level is reached, when indicator below is full.\n");
    printf("\e[01;38;05;242m     x  When any block is pushed out of the grid, game is over.\n\e[0m");
    printf("\n");
    printf("  Instructions:\n");
    printf("     *  Enter number of column, where next block drops.\n");
    printf("\n\n");
    printf("             Enjoy!\e[0m\n\n");
    getchar();
    return 0;
}