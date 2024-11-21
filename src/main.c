#include "api.h"

int main() {
    GameState game;
    initGame(&game);
    setInputMode(&game);
    printf("\033[2J\033[H");

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 50000000;

    while (1) {
        handleInput(&game);
        updateGame(&game);
        drawBoard(&game);
        fflush(stdout);
        nanosleep(&ts, NULL);
    }

    resetInputMode(&game);
    return 0;
}
