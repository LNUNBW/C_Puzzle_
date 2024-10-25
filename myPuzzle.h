#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <stdbool.h>
#include <locale.h>
#include <wchar.h>

#define FINISH_MESSAGE "Finish!!"
#define KEY_UP 'A'
#define KEY_DOWN 'B'
#define KEY_RIGHT 'C'
#define KEY_LEFT 'D'
#define MIN_PUZZLE_SIZE 3
#define MAX_PUZZLE_SIZE 10
#define DEFAULT_SIZE 3

#define CONFIG "./config.txt"
#define SCOREDIR "./score"
#define SCORE "./score/score.txt"

typedef struct{
    int index;
    int *p1D;
}D1_PZ;

typedef struct{
    int row;
    int column;
    int **p2D;
}D2_PZ;

typedef struct{
    int size;
    int test;
    D1_PZ d1;
    D2_PZ d2;
    char dir[200];
    char score[200];
    char user[80];
}MY_PUZZLE;

int p2D_init(int, MY_PUZZLE *);
int p2D_display(int, MY_PUZZLE *);
int p2D_play(int, MY_PUZZLE *);
int p2D_finish(int, MY_PUZZLE *);
int p2D_config(int, MY_PUZZLE *);
int p2D_recordScore(char *, int, char *, char *);
void p2D_printScore(char *, char *);

void displayStars(wchar_t grid[15][15]);    // 별 출력 함수
void fullStars(wchar_t grid[15][15], int step);  // 별 채우기 함수
void emptyStars(wchar_t grid[15][15], int step);  // 별 채우기 함수
void removeStars(wchar_t grid[15][15], int step);  // 별 제거 함수

void p2D_intro(wchar_t grid[15][15]);

void set_input_mode();
int kbhit();
int getKey();
