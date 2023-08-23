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