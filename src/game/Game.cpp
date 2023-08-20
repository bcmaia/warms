#include "Game.hpp"

Game::Game(unsigned long seed, unsigned population_size) : board() {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned long> dis(0, std::numeric_limits<unsigned long>::max());
    
    // Preallocate the agents vector to the desired size
    agents.reserve(population_size);

    // Generate snakes here using the provided population_size
    for (unsigned i = 0; i < population_size; ++i) {
        unsigned long snakeSeed = dis(gen);
        agents.emplace_back(Snake(snakeSeed, Position{0, 0}, 3));
    }
}
