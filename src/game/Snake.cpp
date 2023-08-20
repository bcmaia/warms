#include "Snake.hpp"


Snake::Snake(unsigned long seed, Position initial = Position{0, 0}, unsigned short start_lenght = 3) : genes(GENE_SIZE), gen(seed) {
    // Initialize genes with random values
    std::uniform_real_distribution<float> dis(-1000.0, 1000.0);

    for (size_t i = 0; i < GENE_SIZE; ++i) {
        genes[i] = dis(gen);
    }

    alive = true;
    facing = Direction::Up;

    //body.reserve(8);
    lenght = start_lenght;
    colorPair = 1;
    body.emplace_back(initial);

    // Initialize other member variables if needed
    // Example: initialize body, length, facing, etc.
}


// void Snake::move() {
//     if (1 == body.size() )  {
//         body.push_back();
//     }
// }