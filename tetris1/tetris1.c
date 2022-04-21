#include <stdio.h>
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

int main(int argc, char* argv[])
{
    clearScreen();  // 画面クリア
    setBackColor(BLUE); // 文字色設定
    setCharColor(YELLOW);   // 文字の背景色設定
    setPosition(5, 10);     // 位置指定
    printf("hello!\n");   fflush(stdout); // 表示
    setBackColor(DEFAULT);  // 文字背景色を元に戻す
    setCharColor(DEFAULT);  // 文字色を元に戻す
}