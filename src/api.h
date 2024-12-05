#ifndef API_H
#define API_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// 定数の定義
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 4

// 構造体の定義
typedef struct {
    int shape[BLOCK_SIZE][BLOCK_SIZE];
    int size;
    int colorCode;
} Tetrimino;

typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    int colorBoard[BOARD_HEIGHT][BOARD_WIDTH];
    int currentX, currentY;
    Tetrimino currentTetrimino;
    Tetrimino nextTetrimino;
    int score;
    int level;
    int linesCleared;
    unsigned long lastFallTime;
    double fallInterval;
    struct termios oldt, newt;
} GameState;

// 関数のプロトタイプ宣言
void initGame(GameState *game);
void drawBoard(GameState *game);
void spawnTetrimino(GameState *game);
int checkCollision(GameState *game, int newX, int newY, Tetrimino *tet);
void placeTetrimino(GameState *game);
void rotateTetrimino(GameState *game);
int removeFullLines(GameState *game);
void setInputMode(GameState *game);
void resetInputMode(GameState *game);
int kbhit(void);
void gameOverSequence(GameState *game);
unsigned long getCurrentTimeMs();
void handleInput(GameState *game);
void updateGame(GameState *game);
void endGame(GameState *game);

#endif // API_H
