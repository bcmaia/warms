#pragma once

#include <vector>
#include <random>

#include "types.hpp"


vectorf32 generateRandomVector(int size, unsigned long seed);
matrixf32 generateRandomMatrix(int numRows, int numCols, unsigned long seed);
// vectorf32 mul_matrix_by_vec(const matrixf32& matrix, const vectorf32& vector);