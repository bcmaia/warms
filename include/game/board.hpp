#pragma once

#include <vector>

#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>

#include "types.hpp"

class Board {
    public:
        Board();
        ~Board();

        unsigned short getWidth () const {return width;};
        unsigned short getHeight () const {return height;};

        void printat (Position point, Cell cell);
        void setcell (Position point, Cell cell);
        void render () const;

    private:
        unsigned short width;
        unsigned short height;

        std::unique_ptr<std::vector<std::vector<Cell>>> matrix;
        std::unique_ptr<std::vector<std::vector<Cell>>> auxiliar;

        void swapMatrices() {std::swap(matrix, auxiliar);};
};