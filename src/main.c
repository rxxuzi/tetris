#include "api.h"

int main() {
    GameState game;
    evtp();
    initGame(&game);
    setInputMode(&game);
    printf("\033[2J\033[H");

    while (1) {
        handleInput(&game);
        updateGame(&game);
        drawBoard(&game);
        fflush(stdout);
        sleep_ms(50);
    }

    resetInputMode(&game);
    return 0;
}
