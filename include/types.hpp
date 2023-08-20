#pragma once

typedef int ColorPair;

struct Cell {
    char character;
    ColorPair colorPair;
};

struct Position {
    unsigned short x, y;
};