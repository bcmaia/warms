#pragma once

#include <cstdint>


typedef int ColorPair;

struct Cell {
    char character;
    ColorPair colorPair;
};

struct Position {
    unsigned short x;
    unsigned short y;
};


enum class Direction : uint8_t {
    Up, Down, Right, Left,
};