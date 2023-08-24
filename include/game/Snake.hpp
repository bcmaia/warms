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

#define SHRINK_FACTOR (1.0 / 1000.0)

class Snake {
    public:
        Snake(
            unsigned long seed, 
            Position initial, 
            unsigned short start_lenght
        );
         Snake(
            Genome& parent1,
            Genome& parent2,
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
        // Genome get_genome() {return genome;}
        Genome genome;

        float time_alive;
        bool alive;
        unsigned short lenght;
    private:
        // float time_alive;
        bool moved;
        void die (Board& board);
        
        // std::vector<float> genes;
        Direction facing;
        std::mt19937 gen;
        
        float speed;
        float movement;
        float shrink;

        Position neck;
        Option<Position> new_cell;
        Option<Position> dead_cell;
        Option<Position> dead_cell_2;
};  
