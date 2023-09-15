#pragma once

#include <vector>
#include <random>
#include <cmath>

#include "types.hpp"


vectorf32 generateRandomVector(int size, unsigned long seed);
matrixf32 generateRandomMatrix(int numRows, int numCols, unsigned long seed);
std::vector<float> softmax(const std::vector<float>& input);
// vectorf32 mul_matrix_by_vec(const matrixf32& matrix, const vectorf32& vector);