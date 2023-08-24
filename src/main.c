#include "Game.hpp"
#include <random>

int main() {
    // printf("%f\n", Cell('*', 7).to_float());

    // return 0;
    // //unsigned long seed = 1;
    unsigned population_size = 33;

    std::random_device rd;  // Obtain a random seed from hardware
    std::mt19937_64 eng(rd());  // Seed the random number generator
    std::uniform_int_distribution<unsigned long> dist;
    unsigned long random_value = dist(eng); 

    Game game(random_value, population_size);
    game.run();

    return 0;
}
