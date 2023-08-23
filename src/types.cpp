#include "types.hpp"

// Mapping characters and color pairs to unique numeric values
std::unordered_map<char, int> charMap;
std::unordered_map<int, int> colorMap;

constexpr float CELL_MAP_COLOR_FACTOR = 1.0 / 16.0;
constexpr float CELL_MAP_NORMALIZATION = 1.0 / (255.0 + 255.0 * CELL_MAP_COLOR_FACTOR);

float Cell::to_float () const {
    float char_value = static_cast<float>(character);
    float color_value = static_cast<float>(colorPair);

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

// Overloading - operator for position subtraction
bool Position::operator==(const Position& other) const {
    return (x == other.x) && (y == other.y);
}

// Overloading - operator for position subtraction
bool Position::operator!=(const Position& other) const {
    return (x != other.x) || (y != other.y);
}

// Takes dimentions, asumes they are positive
bool Position::inside(const Position& dimentions) const {
    return (
        (0 <= x)
        && (0 <= y)
        && (dimentions.x > x)
        && (dimentions.y > y)
    );
}

Position Position::mold (const Position& dimentions) const {
    return Position{
        (x % dimentions.x + dimentions.x) % dimentions.x,
        (y % dimentions.y + dimentions.y) % dimentions.y,
    };
}



Direction rotate (Direction d, bool clockwise = true) {
    if (clockwise) switch (d) {
            case Direction::Up:     return Direction::Right;
            case Direction::Right:  return Direction::Down;
            case Direction::Down:   return Direction::Left;
            case Direction::Left:   return Direction::Up;
    } else switch (d) {
        case Direction::Up:     return Direction::Left;
        case Direction::Right:  return Direction::Up;
        case Direction::Down:   return Direction::Right;
        case Direction::Left:   return Direction::Down;
    }

    return d; // error case
}