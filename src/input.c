#include "api.h"

#ifndef _WIN32
void setInputMode(GameState *game) {
    tcgetattr(STDIN_FILENO, &(game->oldt));
    game->newt = game->oldt;
    game->newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &(game->newt));
}

void resetInputMode(GameState *game) {
    tcsetattr(STDIN_FILENO, TCSANOW, &(game->oldt));
}

int kbhitCustom(void) {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
#else
void setInputMode(GameState *game) {
    (void)game;
}

void resetInputMode(GameState *game) {
    (void)game;
}

int kbhitCustom(void) {
    return _kbhit();
}
#endif

void handleInput(GameState *game) {
    if (kbhitCustom()) {
#ifdef _WIN32
        char c = getch();
#else
        char c = getchar();
#endif
        if (c == 'q') {
            resetInputMode(game);
            printf("\033[2J");
            printf("Thank you for playing!\n");
            exit(0);
        } else if (c == 'a') {
            if (!checkCollision(game, game->currentX -1, game->currentY, &game->currentTetrimino))
                game->currentX--;
        } else if (c == 'd') {
            if (!checkCollision(game, game->currentX +1, game->currentY, &game->currentTetrimino))
                game->currentX++;
        } else if (c == 's') {
            if (!checkCollision(game, game->currentX, game->currentY +1, &game->currentTetrimino))
                game->currentY++;
        } else if (c == 'w') {
            rotateTetrimino(game);
        } else if (c == ' ') { // space for hard drop
            while (!checkCollision(game, game->currentX, game->currentY +1, &game->currentTetrimino)) {
                game->currentY++;
            }
        }
    }
}
