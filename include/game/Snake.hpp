#pragma once

#include <vector>
#include <deque>
#include <cstdio>
#include <random>

#include "Board.hpp"
#include "Genome.hpp"

#include "types.hpp"
#include "utils.hpp"

//#define GENE_SIZE (10)

class Snake {
    public:
        Snake(
            unsigned long seed, 
            Position initial, 
            unsigned short start_lenght
        );
        ~Snake () {};

        ColorPair colorPair;
        std::deque<Position> body;

        void move(Board& board, float deltaTime);
        void think(Board& board);
        void shed_dead_cell (Board& board);
        void show_new_cell (Board& board);
        void survival_test(Board& board);
        void render (Board& board);

    private:
        bool moved;
        void die (Board& board);
        unsigned short lenght;
        //std::vector<float> genes;
        Direction facing;
        std::mt19937 gen;

        Genome genome;
        
        float speed;
        float movement;
        bool alive;

        Position neck;
        Option<Position> new_cell;
        Option<Position> dead_cell;
};  
