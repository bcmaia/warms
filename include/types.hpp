#pragma once

#include <cstdint>
#include <unordered_map>
#include <cmath>


typedef unsigned char ColorPair;

struct Cell {
    unsigned char character;
    ColorPair colorPair;
};

class Position {
    public:
        unsigned int x;
        unsigned int y;

        Position() : x(0), y(0) {}
        Position(unsigned x_coord, unsigned y_coord) : x(x_coord), y(y_coord) {}
        Position(int x_coord, int y_coord) {
            x = std::abs(x_coord);
            y = std::abs(y_coord);
        }

        unsigned manhattan_magnitude () const {
            return x + y;
        }

        Position trim (const unsigned int max) const {
            return Position(max < x ? max : x, max < y ? max : y);
        }

        unsigned distance (const Position& p) const {
            return static_cast<unsigned>(
                std::abs(static_cast<int>(p.x) - static_cast<int>(x))
                + std::abs(static_cast<int>(p.y) - static_cast<int>(y))
            );
        }

        Position operator% (const Position& dimentions) const {
            return Position(x % dimentions.x, y % dimentions.y);
        }

        // Overloading + operator for position addition
        Position operator+(const Position& other) const {
            return Position(x + other.x, y + other.y);
        }

        // Overloading - operator for position subtraction
        Position operator-(const Position& other) const {
            return Position(
                static_cast<int>(other.x) - static_cast<int>(x),
                static_cast<int>(other.y) - static_cast<int>(y)
            );
        }

        Position operator*(unsigned int scalar) const {
            return Position(x * scalar, y * scalar);
        }

        bool operator==(const Position& other) const {
            return (other.x == x) && (other.y == y);
        }

        Position interpolate(const Position& start, const Position& end, unsigned amount) const {
            Position dif = (end - start).trim(amount);
            return start + dif;
        }
};

class Line {
    public:
        Position p1;
        Position p2;

        Line () {}
        Line(Position position_1, Position position_2) : p1(position_1), p2(position_2) {}

        unsigned lenght () {return (p2 - p1).manhattan_magnitude();}
};


enum class Direction : uint8_t {
    Up, Down, Right, Left,
};

float cell_map (const Cell cell);