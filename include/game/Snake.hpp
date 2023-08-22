#pragma once

#include <vector>
#include <deque>
#include <cstdio>
#include <random>

#include "types.hpp"
#include "Board.hpp"

#define GENE_SIZE (10)

class Snake {
    public:
        Snake(unsigned long seed, Position initial, unsigned short start_lenght);
        ~Snake () {};
        void move(Board& board, float deltaTime);
        Position check_colision(Board& board);
        void think(Board& board);

        ColorPair colorPair;
        std::deque<Position> body;
    private:
        void die ();
        unsigned short lenght;
        std::vector<float> genes;
        Direction facing;
        std::mt19937 gen;
        
        float speed;
        float movement;
        bool alive;
};  
