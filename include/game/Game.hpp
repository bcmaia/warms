#pragma once

#include <vector>

#include "types.hpp"
#include "Snake.hpp"

class Game {
    public:
        Game(unsigned long seed, unsigned population_size);
        ~Game(){};

        

    private:
        Board board;
        std::vector<Snake> agents;
        std::mt19937 gen;
};