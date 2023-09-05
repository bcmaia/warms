#include <random>
#include "constants.hpp"
#include "Game.hpp"


int main() {
    std::random_device rd;  // Obtain a random seed from hardware
    std::mt19937_64 eng(rd());  // Seed the random number generator
    std::uniform_int_distribution<unsigned long> dist;
    unsigned long seed = dist(eng); 

    Game game(seed, POPULATION_SIZE);
    game.run();

    return 0;
}
