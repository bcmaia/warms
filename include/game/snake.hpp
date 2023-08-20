#pragma once

#include <vector>
#include <deque>
#include <cstdio>

#include "types.hpp"
#include "board.hpp"


class Snake {
    public:
        Snake () {};
        ~Snake () {};
        void move();
        void think(Board& board);

    private:
        std::deque<Position> body;
        unsigned short lenght;
        std::vector<float> genes;
        Direction facing;
};  
