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

        ColorPair colorPair;
        std::deque<Position> body;

        void move(const Board& board, float deltaTime);
        void think(Board& board);
        void shed_dead_cell (Board& board);
        void show_new_cell (Board& board);
        void survival_test(Board& board);
        void render (Board& board);

    private:
        bool moved;
        void die ();
        unsigned short lenght;
        std::vector<float> genes;
        Direction facing;
        std::mt19937 gen;
        
        float speed;
        float movement;
        bool alive;

        Position neck;
        Option<Position> new_cell;
        Option<Position> dead_cell;
};  
