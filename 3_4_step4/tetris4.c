#include <stdio.h>
#include <time.h>

#define clearScreen()       printf("\033[2J")
#define setPosition(x,y)    printf("\033[%d;%dH",(y)+1,(x)*2+1)
#define setCharColor(n)     printf("\033[3%dm",(n))
#define setBackColor(n)     printf("\033[4%dm",(n))

#define cursolOn()          printf("\033[?25h") // カーソルを表示
#define cursolOff()         printf("\033[?25l") // カーソルを非表示
#define WIDTH               10  // フィールドの幅
#define HEIGHT              20  // フィールドの高さ

#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7
#define DEFAULT 9

#define setAttribute(n) printf("\033[%dm",(n))
#define NORMAL 0    // 通常
#define BLIGHT  1   // 明るく
#define DIM     2   // 暗く
#define UNDERBAR 4  // 下線
#define BLINK 5     // 点滅
#define REVERSE 7   // 明暗反転
#define HIDE 8      // 隠れ(見えない)
#define STRIKE 9    // 取り消し線

typedef struct cell
{
    char c; // 表示文字
    int charcolor;  // 表示色
    int backcolor;  // 背景色
    int attribute;  // 文字属性
} Cell;


int wait(int msec);
void initialize(void);  // 画面の初期化
void reset(void);       // 画面の復元
int checkRenge(Cell a, int x, int y);
int printCell(Cell c, int x, int y);
int clearCell(Cell c, int x, int y);

int wait(int msec)
{
    struct timespec r = {0, msec * 1000L * 1000L};
    return nanosleep(&r, NULL);       
}

void initialize(void)
{
    setAttribute(NORMAL);
    setBackColor(BLACK);
    setCharColor(WHITE);
    clearScreen();
    cursolOff();
}

void reset(void)
{
    setBackColor(BLACK);
    setCharColor(WHITE);
    setAttribute(NORMAL);
    clearScreen();
    cursolOn();
}

int checkRenge(Cell a, int x, int y)
{
    if (a.c == '\0' || x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return -1;
    else
        return 0;
}

int printCell(Cell c, int x, int y)
{
    if (checkRenge(c, x, y) == -1)
        return -1;

    setPosition(x, y);
    setAttribute(c.attribute);
    setBackColor(c.backcolor);
    setCharColor(c.charcolor);
    printf("%c%c", c.c, c.c);
    fflush(stdout);

    return 0;
}

int clearCell(Cell c, int x, int y)
{
    if (checkRenge(c, x, y) == -1)
        return -1;

    setPosition(x, y);
    setAttribute(NORMAL);
    setBackColor(BLACK);
    setCharColor(BLACK);
    printf("  ");
    fflush(stdout);

    return 0;
}

int main(int argc, char* argv[])
{
    int y;
    Cell a = {' ', WHITE, BLACK, REVERSE};

    initialize();

    for (y = 1; y < HEIGHT; y++)
    {
        printCell(a, 5, y);
        wait(500);
        clearCell(a, 5, y);
    }
    reset();
}