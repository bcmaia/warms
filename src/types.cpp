#include "types.hpp"

// Mapping characters and color pairs to unique numeric values
std::unordered_map<char, int> charMap;
std::unordered_map<int, int> colorMap;

constexpr float CELL_MAP_COLOR_FACTOR = 1.0 / 16.0;
constexpr float CELL_MAP_NORMALIZATION = 1.0 / (255.0 + 255.0 * CELL_MAP_COLOR_FACTOR);

float cell_map (const Cell cell) {
    float char_value = static_cast<float>(cell.character);
    float color_value = static_cast<float>(cell.colorPair);

    return CELL_MAP_NORMALIZATION * (char_value + CELL_MAP_COLOR_FACTOR * color_value);
}








unsigned Position::manhattan_magnitude () const {
    return x + y;
}

Position Position::trim (const int max) const {
    return Position(max < x ? max : x, max < y ? max : y);
}

Position Position::abs () const {
    return Position(std::abs(x), std::abs(y));
}

unsigned Position::manhattan_distance (const Position& p) const {
    return (*this - p).abs().sum();
}

int Position::sum () const {
    return x + y;
}

Position Position::operator% (const Position& dimentions) const {
    return Position(x % dimentions.x, y % dimentions.y);
}

// Overloading + operator for position addition
Position Position::operator+(const Position& other) const {
    return Position(x + other.x, y + other.y);
}

Position Position::operator+(const int other) const {
    return Position(x + other, y + other);
}

// Overloading - operator for position subtraction
Position Position::operator-(const Position& other) const {
    return Position(x - other.x, y - other.y);
}
