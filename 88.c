#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

#define NLevel 20
#define Version 22  //numbers of commits in repo

int i, j, k;
int field[8][8] = {0};
int horb[8][8] = {0};
int verb[8][8] = {0};
int score = 0;
int next = 0;
int gameover = 0;
int state, number;
int tobechecked = 0;
int fnc = 0;
int indicator = 0;
int LOG = 0;
int DEBUG = 0;
int seed = 0;
int scoreadd;
int levels = 0;
int CHAINS = 0;

int print();
int fall();
int init();
int check();
int destroy();
int newlevel();

void placenew();
int nextblock();
void startscreen();
void revealblock(int, int);
void discloseblock_kj(int, int);
void discloseblock_ik(int, int);

int main(int argc, char const *argv[])
{
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--debug") == 0)
        {
            DEBUG = 1;
        }
        if (strcmp(argv[i], "--chains") == 0)
        {
            CHAINS = 1;
        }
        if (strncmp(argv[i], "--seed=", 7) == 0)
        {
            char tempstring[50];
            for (j = 7; j < strlen(argv[i]); j++)
            {
                tempstring[j - 7] = argv[i][j];
            }
            seed = atoi(tempstring);
        }
        if (strcmp(argv[i], "--help") == 0)
        {
            printf(
                "Eighty-eight\n\n"
                "  --debug        enable debugging features\n"
                "  --seed=SEED    start game with SEED as\n"
                "                 random generator seed\n"
                "  --chains       enable chains visualisation\n"
                "                 not recommended - buggy\n"
                "  --help         show this help and exit\n"
                "  --version      show version and exit\n"
            );
            return 0;
        }
        if (strcmp(argv[i], "--version") == 0)
        {
            printf(
                "Eighty-eight version 0.%d\n"
                "(c) xtotdam  CC-BY-SA\n"
                "e-mail: xtotdam@ya.ru\n"
                , Version);
            return 0;
        }
    }

    startscreen();
    init();
    while (gameover != 1)
    {
        placenew();
        check();
        indicator++;
    }
    printf("Game over!\nYour score: %d\nYou survived %d levels!\n", score, levels);
    return 0;
}

int newlevel()
{
    indicator = 0;
    levels++;
    scoreadd = 1000;
    printf("New level score: +%d\n", scoreadd);
    score += scoreadd;

    if (DEBUG) printf("\e[01;38;05;110N\e[0mew level: ");

    for (i = 0; i < 8; i++)
    {
        if (field[0][i] != 0)
        {
            gameover = 1;
        }
    }

    for (i = 1; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            field[i - 1][j] = field[i][j];
        }
    }

    for (i = 0; i < 8; i++)
    {
        field[7][i] = nextblock();
        if (DEBUG) printf("[%d]", field[7][i]);
    }
    if (DEBUG) printf("\n");
    check();
    return 0;
}

int nextblock()
{
    return 20 * (rand() % 2) + rand() % 9 + 1;
}

void placenew()  //calls print(),nextblock()
{
    int place = 0;

    next = nextblock();
    print();

    printf(" > ");
    scanf("%d", &place);

    if (field[0][place - 1] != 0) gameover = 1;

    if ((place < 0) || (abs(place) == 0))
    {
        place += (place / 8 + 1) * 8;
    }
    place = (place - 1) % 8 + 1;    //for negatives and zero

    field[0][place - 1] = next;
    if (DEBUG) printf("\e[01;38;05;110mA\e[0mdding...   [%d] > %d\n", next, place);
}

int print() //doesn't call any other functions
{
    if (DEBUG)  printf("    _________________________________________________________________\n");
    if (!DEBUG) printf("    _________________________\n");
    for (i = 0; i < 8; i++)
    {
        if (DEBUG) printf(" %d | ", i);
        if (!DEBUG) printf(" %d | ", 8 - i);
        for (j = 0; j < 8; j++)
        {
            state = field[i][j] / 10;
            number = field[i][j] % 10;
            if (field[i][j] == 19) field[i][j] = 9;
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
    printf("     < ");
    for (i = 0; i < (NLevel - indicator); i++) printf("=");
    for (i = 0; i < indicator; i++) printf("\e[01;38;05;242m-");
    if (DEBUG) printf("\e[0m : %d", indicator);
    printf("\e[0m >\n");
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

int init()  //calls fall(),print(),check(),nextblock();
{
    int timeseed = time(NULL);

    if (seed == 0)
    {
        seed = timeseed;
    }
    srand(seed);
    printf("seed:  %d\n", seed);
    if (seed == 1) printf("seed 1 : 7 4 4 6 5 6 4 2 3 1 1 2 2 => 2900\n");

    for (i = 0; i < 20; i++)
    {
        field[rand() % 8][rand() % 8] = nextblock();
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

    fall();
    printf("---\n");
    if (CHAINS) print();    //it is useful to show chains, but is buggy now, so --chains
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (field[i][j] == 19)
            {
                field[i][j] = 9;    //because no bombs can be grey
                if (DEBUG) printf("\e[01;38;05;222mI\e[0mgnite...   %d%d : (29 >) 19 > 9\n", i, j);
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
                    if (field[k][j] == 0) break; up++;
                }
                for (k = j + 1; k < 8; k++)
                {
                    if (field[i][k] == 0) break; right++;
                }
                for (k = i + 1; k < 8; k++)
                {
                    if (field[k][j] == 0) break; down++;
                }
                for (k = j - 1; k > (-1); k--)
                {
                    if (field[i][k] == 0) break; left++;
                }
                if (DEBUG) printf(": %du %dr %dd %dl ", up, right, down, left);
                horb[i][j] = right + left + 1;
                verb[i][j] = up + down + 1;
                if (DEBUG) printf("| %dh %dv\n", horb[i][j], verb[i][j]);
                //maybe not-zeroing horb[][] and verb[][] can cause a bug, when already not
                //actual values in them will cause another blocks to disappear.
                //maybe. it was not proven experimentally.
            }
        }
    }
    destroy();
    if (indicator == NLevel) newlevel();
    return 0;
}

int destroy()   //calls check() if destroys a block
{
    fnc = 0;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            state  = field[i][j] / 10; number = field[i][j] % 10;

            if ((field[i][j] != 0) && (state == 0) && (number != 0) && (number != 9))
            {
                if ((field[i][j] == horb[i][j]) || (field[i][j] == verb[i][j]))
                {
                    if (DEBUG) printf("\e[01;38;05;196mD\e[0mestroy...  %d %d [%d] %dh %dv\n", i, j, field[i][j], horb[i][j], verb[i][j]);
                    scoreadd = field[i][j] % 10 * 10;
                    printf("Block destroy score: +%d\n", scoreadd);
                    score += scoreadd;
                    field[i][j] = 0;
                    fnc = 1;

                    revealblock(-1, 0); //N
                    revealblock(0, 1);  //E
                    revealblock(1, 0);  //S
                    revealblock(0, -1); //W

                    for (k = 0; k < 8; k++)
                    {
                        if (field[k][j] == 9)
                        {
                            field[k][j] = 0;
                            if (DEBUG) printf("\e[01;38;05;196mE\e[0mxplode...  %d %d [@]\n", k, j);

                            discloseblock_kj(0, -1); //N
                            discloseblock_kj(1, -1); //NE
                            discloseblock_kj(1, 0); //E
                            discloseblock_kj(1, 1); //SE
                            discloseblock_kj(0, 1); //S
                            discloseblock_kj(-1, 1); //SW
                            discloseblock_kj(-1, 0); //W
                            discloseblock_kj(-1, -1); //NW
                        }

                        if (field[i][k] == 9)
                        {
                            field[i][k] = 0;
                            if (DEBUG) printf("\e[01;38;05;196mE\e[0mxplode...  %d %d [@]\n", i, k);

                            discloseblock_ik(-1, 0); //N
                            discloseblock_ik(-1, 1); //NE
                            discloseblock_ik(0, 1); //E
                            discloseblock_ik(1, 1); //SE
                            discloseblock_ik(1, 0); //S
                            discloseblock_ik(1, -1); //SW
                            discloseblock_ik(0, -1); //W
                            discloseblock_ik(-1, -1); //NW
                        }
                    }
                }
            }
        }
    }
    if (fnc == 1) check();
    return 0;
}

void startscreen()   //doesn't call any other functions
{
    printf(
        "  **                                                               **   \n"
        "  *                          Eighty  Eight                          *   \n"
        "  **                                                               **   \n"
        "\n\n"
        "  Rules:\n"
        "\e[01;38;05;107m     v  Block disappears, if its number equals amount\n"
        "         of blocks in the same row/column.\n"
        "\e[01;38;05;107m     v  Gray blocks are uncovered by breaking\n"
        "         touching blocks.\n"
        "\e[01;38;05;107m     v  Bomb can be triggered by breaking block in\n"
        "         the same row/column.\n"
        "\e[01;38;05;107m     v  When block disappears, block above will drop.\n"
        "\e[01;38;05;107m     v  New level is reached, when indicator below is full.\n"
        "\e[01;38;05;242m     x  When any block is pushed out of the grid,\n"
        "         game is over.\n\e[0m"
        "\n"
        "  Instructions:\n"
        "\e[01;38;05;107m     v  Enter number of column, where next block drops.\n"
        "\e[01;38;05;107m     v  Use --debug, --seed=SEED, --chains arguments.\n"
        "\e[01;38;05;107m     v  To exit input any letter.\n"
        "\n\n"
        "\e[01;38;05;222m             Enjoy!\e[0m\n\n"
    );
    getchar();
}

void revealblock(int _di, int _dj)
{
    int _newi = i + _di;
    int _newj = j + _dj;
    if ((_newi >= 0) && (_newi < 8) && (_newj >= 0) && (_newj < 8) && (field[_newi][_newj] > 9))
    {
        field[_newi][_newj] -= 10;
        printf("Block reveal score: +%d\n", 100);
        score += 100;
        if (DEBUG) printf("\e[01;38;05;196mR\e[0meveal...   %d%d : %d > %d\n",
                              _newi, _newj, field[_newi][_newj] + 10, field[_newi][_newj]);
    }
}

void discloseblock_kj(int _dk, int _dj)
{
    int _newk = k + _dk;
    int _newj = j + _dj;
    if ((_newk >= 0) && (_newk < 8) && (_newj >= 0) && (_newj < 8) && (field[_newk][_newj] > 9))
    {
        field[_newk][_newj] -= 10;
        printf("Bomb disclose score: +%d\n", 50);
        score += 50;
        if (DEBUG) printf("\e[01;38;05;196mD\e[0misclose... %d%d : %d > %d\n",
                              _newk, _newj, field[_newk][_newj] + 10, field[_newk][_newj]);
    }
}

void discloseblock_ik(int _di, int _dk)
{
    int _newi = i + _di;
    int _newk = k + _dk;
    if ((_newi >= 0) && (_newi < 8) && (_newk >= 0) && (_newk < 8) && (field[_newi][_newk] > 9))
    {
        field[_newi][_newk] -= 10;
        printf("Bomb disclose score: +%d\n", 50);
        score += 50;
        if (DEBUG) printf("\e[01;38;05;196mD\e[0misclose... %d%d : %d > %d\n",
                              _newi, _newk, field[_newi][_newk] + 10, field[_newi][_newk]);
    }
}