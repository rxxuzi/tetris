#include "api.h"

void setInputMode(GameState *game) {
    tcgetattr(STDIN_FILENO, &(game->oldt));
    game->newt = game->oldt;
    game->newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &(game->newt));
}

void resetInputMode(GameState *game) {
    tcsetattr(STDIN_FILENO, TCSANOW, &(game->oldt));
}

int kbhit(void) {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

void handleInput(GameState *game) {
    if (kbhit()) {
        char c = getchar();
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
        } else if (c == ' ') {
            while (!checkCollision(game, game->currentX, game->currentY +1, &game->currentTetrimino)) {
                game->currentY++;
            }
        }
    }
}
