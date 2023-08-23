#pragma once

#include <cstdint>
#include <unordered_map>
#include <cmath>
#include <stdexcept>


typedef unsigned char ColorPair;

struct Cell {
    unsigned char character;
    ColorPair colorPair;
};

class Position {
    public:
        int x;
        int y;

        Position() : x(0), y(0) {}
        Position(unsigned x_coord, unsigned y_coord) : x(x_coord), y(y_coord) {}
        Position(int x_coord, int y_coord) : x(x_coord), y(y_coord) {}

        int sum () const;
        unsigned manhattan_magnitude () const;
        Position abs () const;

        unsigned manhattan_distance (const Position& p) const;
        Position trim (const int max) const;

        Position operator% (const Position& dimentions) const;
        Position operator+(const Position& other) const;
        Position operator+(const int other) const;
        Position operator-(const Position& other) const;
};

enum class Direction : uint8_t {
    Up, Down, Right, Left,
};

float cell_map (const Cell cell);


template <typename T> class Option {
    private:
        bool __has_value;
        T __value;

    public:
        Option() : __has_value(false) {}
        Option(T val) : __has_value(true), __value(val) {}

        void set_value (T new_value) {
            __value = new_value;
            __has_value = true;
        }

        void clear () {
            __has_value = false;
        }

        bool has_value () const {
            return __has_value;
        }

        T unwrap() const {
            if (__has_value) {
                return __value;
            } else {
                throw std::runtime_error("Tried to unwrap None");
            }
        }

        T unwrap_or(T alternative) const {
            return __has_value ? __value : alternative;
        }
};
