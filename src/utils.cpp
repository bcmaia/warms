#include "utils.hpp"


matrixf32 generateRandomMatrix(int numRows, int numCols, unsigned long seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dis(-1.0, 1.0);

    matrixf32 matrix(numRows, vectorf32(numCols));

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    return matrix;
}


vectorf32 generateRandomVector(int size, unsigned long seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dis(-1.0, 1.0);

    vectorf32 vector(size);

    for (int i = 0; i < size; ++i) {
        vector[i] = dis(gen);
    }

    return vector;
}

std::vector<float> softmax(const std::vector<float>& input) {
    std::vector<float> result;
    float sum = 0.0f;

    for (float val : input) {
        result.push_back(std::exp(val));
        sum += std::exp(val);
    }

    for (float& val : result) {
        val /= sum;
    }

    return result;
}



// vectorf32 mul_matrix_by_vec(const matrixf32& matrix, const vectorf32& vector, const vectorf32& addens) {
//     std::vector<float> result(3);

//     if (matrix.size() != 3 || matrix[0].size() != 25 || vector.size() != 25 || addens.size != 3) {
//         // Dimensions do not match, return an empty result or handle the error as needed.
//         throw std::runtime_error("Sizes dont match.")
//     }

//     for (size_t i = 0; i < 3; ++i) {
//         for (size_t j = 0; j < 25; ++j) {
//             result[i] += matrix[i][j] * vector[j] + addens[i];
//         }
//     }

//     return result;
// }