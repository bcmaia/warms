#include "Game.hpp"


int main() {
    unsigned long seed = 1;
    unsigned population_size = 10;

    Game game(seed, population_size);
    game.run();

    return 0;
}
