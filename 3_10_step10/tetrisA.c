#include <stdio.h>
#include <time.h>

#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
extern int errno;
struct termios otty, ntty;

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
#define BLOCK_SIZE 4
#define BLOCK_NUM   7

typedef struct cell
{
    char c; // 表示文字
    int charcolor;  // 表示色
    int backcolor;  // 背景色
    int attribute;  // 文字属性
} Cell;

Cell block_type[BLOCK_NUM][BLOCK_SIZE][BLOCK_SIZE] = {
    /* 0 */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL,     /* □□■□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL,     /* □■■□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,    /* □■□□ */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */

    /* 1 */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL,     /* □■■□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */

    /* 2 */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL,     /* □■■□ */
    ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* ■■□□ */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */

    /* 3 */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL,     /* □■■□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL,     /* □■■□ */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */

    /* 4 */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */

    /* 5 */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */
    ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL,     /* ■■■□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */

    /* 6 */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □■□□ */
    '\0', RED, BLACK, NORMAL, ' ', RED, BLACK, REVERSE, ' ', RED, BLACK, REVERSE, '\0', RED, BLACK, NORMAL,     /* □■■□ */
    '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL, '\0', RED, BLACK, NORMAL,     /* □□□□ */
};

int wait_ms(int msec);
void initialize(void);  // 画面の初期化
void reset(void);       // 画面の復元
int checkRenge(Cell a, int x, int y);
int printCell(Cell c, int x, int y);
int clearCell(Cell c, int x, int y);
void copyBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE]);
int printBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
int clearBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);

int kbhit(void);    // キー入力があったかどうか確認
int getch(void);    // キー入力1文字読み込み
int tinit(void);    // 端末の初期化

void rotateBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE]);

int wait_ms(int msec)
{
    struct timespec r = {0, msec * 1000L * 1000L};
    return nanosleep(&r, NULL);
}

void initialize(void)
{
    tinit(); // 端末の初期化
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
    tcsetattr(1, TCSADRAIN, &otty); // 端末の復元
    write(1, "\n", 1);              // 後始末
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

void copyBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE])
{
    int i, j;

    for (j = 0; j < BLOCK_SIZE; j++)
        for (i = 0; i < BLOCK_SIZE; i++)
            dst[j][i] = src[j][i];
}

int printBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y)
{
    int i, j;

    for (j = 0; j < BLOCK_SIZE; j++)
        for (i = 0; i < BLOCK_SIZE; i++)
            printCell(block[j][i], i+x, j+y);

    return 0;
}

int clearBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y)
{
    int i, j;

    for (j = 0; j < BLOCK_SIZE; j++)
        for (i = 0; i < BLOCK_SIZE; i++)
            clearCell(block[j][i], i+x, j+y);

    return 0;
}

// キー入力があったかどうか確認
int kbhit(void)
{
    int ret;
    fd_set rfd;
    struct timeval timeout = {0, 0};

    FD_ZERO(&rfd);
    FD_SET(0, &rfd);    // 0:stdin

    ret = select(1, &rfd, NULL, NULL, &timeout);
    if (ret == 1)
        return 1;
    else
        return 0;        
}

// キー入力1文字読み込み
int getch(void)
{
    unsigned char c;
    int n;

    while ((n = read(0, &c, 1)) < 0 && errno == EINTR);
    if (n == 0) {
        return -1;
    }
    else {
        return (int)c;
    }
}

static void onsignal(int sig)
{
    signal(sig, SIG_IGN);
    switch (sig)
    {
    case SIGINT:
    case SIGQUIT:
    case SIGTERM:
    case SIGHUP:
        exit(1);
        break;
    }
}

// 端末の初期化
int tinit(void)
{
    if (tcgetattr(1, &otty) < 0)
        return -1;
    ntty = otty;
    ntty.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF | ISTRIP);
    ntty.c_oflag &= ~OPOST;
    ntty.c_lflag &= ~(ICANON | ECHO);
    ntty.c_cc[VMIN] = 1;
    ntty.c_cc[VTIME] = 0;
    tcsetattr(1, TCSADRAIN, &ntty);
    signal(SIGINT, onsignal);
    signal(SIGQUIT, onsignal);
    signal(SIGTERM, onsignal);
    signal(SIGHUP, onsignal);

    return 0;
}

void rotateBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE]){
    int i, j;

    for (j = 0; j < BLOCK_SIZE; j++)
        for (i = 0; i < BLOCK_SIZE; i++)
            dst[i][BLOCK_SIZE-1 - j] = src[j][i];
}


int main(int argc, char* argv[])
{
    int x, y, c, count, prex, prey, t;
    Cell block[BLOCK_SIZE][BLOCK_SIZE], block_tmp[BLOCK_SIZE][BLOCK_SIZE];
    struct timeval start_time, now_time, pre_time;
    double duration, thold;

    t = rand() % BLOCK_NUM;
    copyBlock(block_type[t], block);
    initialize();

    x = 5;
    y = 0;

    thold = 0.5;    // 落下の時間間隔
    printBlock(block, x, y); // 初期表示
    gettimeofday(&start_time, NULL);
    pre_time = start_time;  // 前回落下時刻 最初は開始時刻

    for ( ; ; ) {
        prex = x;
        prey = y;

        if (kbhit() != 0) {
            c = getch();
            if (c == 0x1b) {
                c = getch();
                if (c == 0x5b) {
                    c = getch();
                    switch (c)
                    {
                    case 0x41: /* UP */
                        rotateBlock(block, block_tmp);  // ブロック回転
                        clearBlock(block, x, y);        // 元のブロックを画面から消去
                        printBlock(block_tmp, x, y);    // 回転したブロックを表示
                        copyBlock(block_tmp, block);    // 回転したブロックを元のブロックに上書き
                        break;
                    case 0x42: /* DOWN */
                        break;
                    case 0x43:  /* RIGHT */
                        x++;
                        break;
                    case 0x44:  /* LEFT */
                        x--;
                        break;                    
                    default:
                        break;
                    }
                }    
            } else {    // 矢印キー以外の場合
                reset();
                exit(1);
            }
        }

        gettimeofday(&now_time, NULL);
        // 前回からの経過時間
        duration = now_time.tv_sec - pre_time.tv_sec
            + (now_time.tv_usec - pre_time.tv_usec) / 1000000.0;

        // 落下時間間隔以上に時間が経過していないか
        if (duration > thold) { 
            pre_time = now_time;    // 前回落下時間を現在時刻に
            if (y < HEIGHT)
                y++;
            else {
                y = 0;
                x = 5;
                t = rand() % BLOCK_NUM;
                copyBlock(block_type[t], block);
            }
        }

        // ブロックが左右移動・落下していた場合
        if (prex != x || prey != y) {
            clearBlock(block, prex, prey);  // 前回位置のブロックを消す
            printBlock(block, x, y);        // 新しい場所に表示
        }

    }
    reset();
}