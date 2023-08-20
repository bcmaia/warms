#include "Snake.hpp"


Snake::Snake(unsigned long seed) : genes(GENE_SIZE), gen(seed) {
    // Initialize genes with random values
    std::uniform_real_distribution<float> dis(-1000.0, 1000.0);

    for (size_t i = 0; i < GENE_SIZE; ++i) {
        genes[i] = dis(gen);
    }

    // Initialize other member variables if needed
    // Example: initialize body, length, facing, etc.
}