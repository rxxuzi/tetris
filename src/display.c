#include "api.h"

void drawBoard(GameState *game) {
    printf("\033[H");
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < BOARD_WIDTH; x++) {
            int cell = game->board[y][x];
            int color = game->colorBoard[y][x];
            int occupied = 0;
            int currentColor = game->currentTetrimino.colorCode;
            for (int ty = 0; ty < game->currentTetrimino.size; ty++) {
                for (int tx = 0; tx < game->currentTetrimino.size; tx++) {
                    if (game->currentTetrimino.shape[ty][tx]) {
                        int boardX = game->currentX + tx;
                        int boardY = game->currentY + ty;
                        if (boardX == x && boardY == y) {
                            occupied = 1;
                            break;
                        }
                    }
                }
                if (occupied)
                    break;
            }
            if (cell) {
                printf("\033[%dm[]\033[0m", color);
            } else if (occupied && y >=0) {
                printf("\033[%dm[]\033[0m", currentColor);
            } else {
                printf("  ");
            }
        }
        printf("|\n");
    }
    for (int x =0; x<BOARD_WIDTH +2; x++) printf("--");
    printf("\n");
    printf("Score: %d\tLevel: %d\n", game->score, game->level);
    printf("Next:\n");
    for (int y =0; y<BLOCK_SIZE; y++) {
        printf("  ");
        for (int x=0; x<BLOCK_SIZE; x++) {
            if (game->nextTetrimino.shape[y][x]) {
                printf("\033[%dm[]\033[0m", game->nextTetrimino.colorCode);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("Controls: W: Rotate, A: Left, S: Down, D: Right, Space: Drop\n");
}

void gameOverSequence(GameState *game) {
    int invasionHeight = 0;
    int invasionColor = 90;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 200000000;

    while (invasionHeight < BOARD_HEIGHT) {
        for (int y = BOARD_HEIGHT - invasionHeight -1; y >= 0; y--) {
            for (int x =0; x<BOARD_WIDTH; x++) {
                if (y >= BOARD_HEIGHT - invasionHeight -1) {
                    game->board[y][x] = 1;
                    game->colorBoard[y][x] = invasionColor;
                } else {
                    game->board[y][x] = 0;
                    game->colorBoard[y][x] = 0;
                }
            }
        }
        drawBoard(game);
        printf("Game Over\n");
        fflush(stdout);
        nanosleep(&ts, NULL);
        invasionHeight++;
    }

    printf("\033[2J\033[H");
    printf("Game Over\n");
    printf("Final Score: %d\n", game->score);
    printf("Press R to Restart, Q to Quit\n");
    fflush(stdout);

    while (1) {
        if (kbhit()) {
            char c = getchar();
            if (c == 'q' || c == 'Q') {
                resetInputMode(game);
                printf("\033[2J");
                printf("Thank you for playing!\n");
                exit(0);
            } else if (c == 'r' || c == 'R') {
                initGame(game);
                printf("\033[2J\033[H");
                break;
            }
        }
        nanosleep(&ts, NULL);
    }
}
