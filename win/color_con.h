//by xtotdam  cc-by-sa =)

//try colourdemo() to learn colours
//setcolour(colour text, colour background)

//aliases:  clrclr() - back to default,
//          errfont() - red on black
//          b_w() - white on black
//          setbg_w(int) - white text
//          setbg_b(int) - black text
//          settxt(int) - black background

#include<conio.h>
#include<stdio.h>
#include<Windows.h>

enum colnames{
    Black = 0,
    DarkBlue,
    DarkGreen,
    DarkCyan,
    DarkRed,
    DarkMagenta,
    Brown,
    Gray,
    DarkGray,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Yellow,
    White
};

void setcolour(int text, int background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
void clrclr(void)
{
    setcolour(Gray, Black);
}
void errfont(void)
{
    setcolour(White, Red);
}
void setbg_w(int background)
{
    setcolour(White, background);
}
void setbg_b(int background)
{
    setcolour(Black, background);
}
void b_w(void)
{
    setcolour(White, Black);
}
void settxt(int text)
{
    setcolour(text, Black);
}
void colourdemo (void)
{
    setcolour(15, 0);    printf(" 0:  Black"); setcolour(0, 0); printf("              \n");
    setcolour(1, 0);     printf(" 1:  DarkBlue"); setcolour(0, 1); printf("           \n");
    setcolour(2, 0);     printf(" 2:  DarkGreen"); setcolour(0, 2); printf("          \n");
    setcolour(3, 0);     printf(" 3:  DarkCyan"); setcolour(0, 3); printf("           \n");
    setcolour(4, 0);     printf(" 4:  DarkRed"); setcolour(0, 4); printf("            \n");
    setcolour(5, 0);     printf(" 5:  DarkMagenta"); setcolour(0, 5); printf("        \n");
    setcolour(6, 0);     printf(" 6:  Brown"); setcolour(0, 6); printf("              \n");
    setcolour(7, 0);     printf(" 7:  Gray"); setcolour(0, 7); printf("               \n");
    setcolour(8, 0);     printf(" 8:  DarkGray"); setcolour(0, 8); printf("           \n");
    setcolour(9, 0);     printf(" 9:  Blue"); setcolour(0, 9); printf("               \n");
    setcolour(10, 0);    printf("10:  Green"); setcolour(0, 10); printf("              \n");
    setcolour(11, 0);    printf("11:  Cyan"); setcolour(0, 11); printf("               \n");
    setcolour(12, 0);    printf("12:  Red"); setcolour(0, 12); printf("                \n");
    setcolour(13, 0);    printf("13:  Magenta"); setcolour(0, 13); printf("            \n");
    setcolour(14, 0);    printf("14:  Yellow"); setcolour(0, 14); printf("             \n");
    setcolour(15, 0);    printf("15:  Black"); setcolour(0, 15); printf("              \n");
    clrclr();
}
