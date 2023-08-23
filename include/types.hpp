#pragma once

#include <cstdint>
#include <unordered_map>


typedef unsigned char ColorPair;

struct Cell {
    unsigned char character;
    ColorPair colorPair;
};

struct Position {
    unsigned short x;
    unsigned short y;
};


enum class Direction : uint8_t {
    Up, Down, Right, Left,
};

float cell_map (const Cell cell);