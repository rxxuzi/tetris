#include "api.h"

static Tetrimino minos[] = {
    {{
        {1,1,1,1}, 
        {0,0,0,0}, 
        {0,0,0,0}, 
        {0,0,0,0}}, 4, 36}, // I
    {{
        {1,1,0,0}, 
        {1,1,0,0}, 
        {0,0,0,0}, 
        {0,0,0,0}}, 2, 33}, // O
    {{
        {0,1,0,0}, 
        {1,1,1,0}, 
        {0,0,0,0}, 
        {0,0,0,0}}, 3, 35}, // T
    {{
        {1,0,0,0}, 
        {1,0,0,0}, 
        {1,1,0,0}, 
        {0,0,0,0}}, 3, 37}, // L
    {{
        {0,0,1,0}, 
        {0,0,1,0}, 
        {0,1,1,0}, 
        {0,0,0,0}}, 3, 34}, // J
    {{
        {0,1,1,0}, 
        {1,1,0,0}, 
        {0,0,0,0}, 
        {0,0,0,0}}, 3, 32}, // S
    {{
        {1,1,0,0}, 
        {0,1,1,0}, 
        {0,0,0,0}, 
        {0,0,0,0}}, 3, 31}  // Z
};

void initGame(GameState *game) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            game->board[y][x] = 0;
            game->colorBoard[y][x] = 0;
        }
    }
    game->score = 0;
    game->level = 1;
    game->linesCleared = 0;
    game->fallInterval = 1000.0;
    game->lastFallTime = getCurrentTimeMs();
    srand((unsigned)time(NULL));
    game->nextTetrimino = minos[rand() % 7];
    spawnTetrimino(game);
}

void spawnTetrimino(GameState *game) {
    game->currentTetrimino = game->nextTetrimino;
    game->nextTetrimino = minos[rand() % 7];
    game->currentX = BOARD_WIDTH / 2 - game->currentTetrimino.size / 2;
    game->currentY = 0;
}

int checkCollision(GameState *game, int newX, int newY, Tetrimino *tet) {
    for (int y = 0; y < tet->size; y++) {
        for (int x = 0; x < tet->size; x++) {
            if (tet->shape[y][x]) {
                int boardX = newX + x;
                int boardY = newY + y;
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                    return 1;
                }
                if (boardY >= 0 && game->board[boardY][boardX]) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void placeTetrimino(GameState *game) {
    for (int y = 0; y < game->currentTetrimino.size; y++) {
        for (int x = 0; x < game->currentTetrimino.size; x++) {
            if (game->currentTetrimino.shape[y][x]) {
                int boardX = game->currentX + x;
                int boardY = game->currentY + y;
                if (boardY >= 0 && boardY < BOARD_HEIGHT && boardX >= 0 && boardX < BOARD_WIDTH) {
                    game->board[boardY][boardX] = 1;
                    game->colorBoard[boardY][boardX] = game->currentTetrimino.colorCode;
                }
            }
        }
    }
}

void rotateTetrimino(GameState *game) {
    Tetrimino rotated;
    rotated.size = game->currentTetrimino.size;
    rotated.colorCode = game->currentTetrimino.colorCode;
    for (int y = 0; y < game->currentTetrimino.size; y++) {
        for (int x = 0; x < game->currentTetrimino.size; x++) {
            rotated.shape[y][x] = game->currentTetrimino.shape[game->currentTetrimino.size - x - 1][y];
        }
    }
    int wallKickOffsets[][2] = {
        {0,0},{-1,0},{1,0},{-2,0},{2,0},{0,-1},
    };
    int numOffsets = (int)(sizeof(wallKickOffsets) / sizeof(wallKickOffsets[0]));
    for (int i = 0; i < numOffsets; i++) {
        int newX = game->currentX + wallKickOffsets[i][0];
        int newY = game->currentY + wallKickOffsets[i][1];
        if (!checkCollision(game, newX, newY, &rotated)) {
            game->currentTetrimino = rotated;
            game->currentX = newX;
            game->currentY = newY;
            break;
        }
    }
}

int removeFullLines(GameState *game) {
    int linesRemoved = 0;
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int full = 1;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!game->board[y][x]) {
                full = 0;
                break;
            }
        }
        if (full) {
            linesRemoved++;
            game->linesCleared++;
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    game->board[yy][x] = game->board[yy -1][x];
                    game->colorBoard[yy][x] = game->colorBoard[yy -1][x];
                }
            }
            for (int x = 0; x < BOARD_WIDTH; x++) {
                game->board[0][x] = 0;
                game->colorBoard[0][x] = 0;
            }
            y--;
        }
    }
    return linesRemoved;
}

void updateGame(GameState *game) {
    unsigned long currentTime = getCurrentTimeMs();
    if (currentTime - game->lastFallTime >= (unsigned long)game->fallInterval) {
        if (!checkCollision(game, game->currentX, game->currentY +1, &game->currentTetrimino)) {
            game->currentY++;
        } else {
            placeTetrimino(game);
            int lines = removeFullLines(game);
            if (lines > 0) {
                game->score += lines * 100;
                if (game->linesCleared >= game->level * 10) {
                    game->level++;
                    if (game->fallInterval > 100)
                        game->fallInterval -= 100;
                }
            }
            spawnTetrimino(game);
            if (checkCollision(game, game->currentX, game->currentY, &game->currentTetrimino)) {
                endGame(game);
            }
        }
        game->lastFallTime = currentTime;
    }
}

void endGame(GameState *game) {
    gameOverSequence(game);
}

unsigned long getCurrentTimeMs() {
#ifdef _WIN32
    return (unsigned long)GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long)(tv.tv_sec) * 1000 + (unsigned long)(tv.tv_usec) / 1000;
#endif
}

void sleep_ms(unsigned int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms/1000;
    ts.tv_nsec = (ms%1000)*1000000;
    nanosleep(&ts, NULL);
#endif
}
