#include <stdio.h>
#include <time.h>

#define clearScreen()       printf("\033[2J")
#define setPosition(x,y)    printf("\033[%d;%dH",(y),(x))
#define setCharColor(n)     printf("\033[3%dm",(n))
#define setBackColor(n)     printf("\033[4%dm",(n))

#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7
#define DEFAULT 9

int wait(int msec);

int wait(int msec)
{
    struct timespec r = {0, msec * 1000L * 1000L};
    return nanosleep(&r, NULL);       
}

int main(int argc, char* argv[])
{
    int y;

    clearScreen();  // 画面クリア
    setBackColor(BLUE); // 文字色設定
    setCharColor(YELLOW);   // 文字の背景色設定

    for (y = 1; y < 23; y++)    // 1行ずつ繰り返す(23回)
    {
        setPosition(5, y);
        printf("$");   fflush(stdout);
        wait(500);
    }

    setBackColor(DEFAULT);  // 文字背景色を元に戻す
    setCharColor(DEFAULT);  // 文字色を元に戻す
}