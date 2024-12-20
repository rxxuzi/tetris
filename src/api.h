#ifndef API_H
#define API_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 4

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
#ifndef _WIN32
    struct termios oldt, newt;
#endif
} GameState;

void initGame(GameState *game);
void evtp(void);
void drawBoard(GameState *game);
void spawnTetrimino(GameState *game);
int checkCollision(GameState *game, int newX, int newY, Tetrimino *tet);
void placeTetrimino(GameState *game);
void rotateTetrimino(GameState *game);
int removeFullLines(GameState *game);

void setInputMode(GameState *game);
void resetInputMode(GameState *game);
int kbhitCustom(void);

void gameOverSequence(GameState *game);
unsigned long getCurrentTimeMs();
void handleInput(GameState *game);
void updateGame(GameState *game);
void endGame(GameState *game);

void sleep_ms(unsigned int ms);

#endif // API_H
