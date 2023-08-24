#pragma once

#include <vector>

#include "utils.hpp"
#include "types.hpp"


#include <vector>
#include <random>
#include <stdexcept>

class Genome {
    public:
        matrixf32 mind_factor;
        vectorf32 mind_addends;

        ~Genome(){};

        Genome(unsigned long seed) {
            // Constructor to initialize the matrix and vector
            mind_factor = generateRandomMatrix(3, 25, seed);
            mind_addends = generateRandomVector(3, seed);
        }

        void mutate(float mutationProbability, unsigned long seed) {
            std::mt19937 gen(seed);
            std::uniform_real_distribution<float> dis(-1.0, 1.0);
            std::bernoulli_distribution mutateDist(mutationProbability);

            for (size_t i = 0; i < 3; ++i) {
                for (size_t j = 0; j < 25; ++j) {
                    if (mutateDist(gen)) {
                        mind_factor[i][j] = dis(gen);
                    }
                }
                if (mutateDist(gen)) {
                    mind_addends[i] = dis(gen);
                }
            }
        }

        Genome(const Genome& parent1, const Genome& parent2, unsigned long seed) {
            // Constructor for crossover between two Genome
            std::mt19937 gen(seed);
            std::uniform_int_distribution<size_t> dis(0, 1); // 0 or 1 randomly

            mind_factor = matrixf32(3, vectorf32(25, 0.0));
            mind_addends = vectorf32(3, 0.0);

            for (size_t i = 0; i < 3; ++i) {
                for (size_t j = 0; j < 25; ++j) {
                    mind_factor[i][j] = (1 == dis(gen)) ? parent1.mind_factor[i][j] : parent2.mind_factor[i][j];
                }
                mind_addends[i] = (1 == dis(gen)) ? parent1.mind_addends[i] : parent2.mind_addends[i]; 
            }

            mutate(0.01, seed);
        }

        vectorf32 think (const vectorf32& sensorial_input) {
            vectorf32 result(3);
            
            #ifndef RELEASE
                if (
                    mind_factor.size() != 3 
                    || mind_factor[0].size() != 25
                    || sensorial_input.size() != 25 
                    || mind_addends.size() != 3
                ) {
                    throw std::runtime_error(
                        "Sizes don't match: mind_factor[" + std::to_string(mind_factor.size()) 
                            + "][" + std::to_string(mind_factor[0].size()) 
                            + "], sensorial_input[" 
                            + std::to_string(sensorial_input.size()) 
                            + "], mind_addends[" 
                            + std::to_string(mind_addends.size()) 
                            + "]"
                    );
                }
            #endif

            for (size_t i = 0; i < 3; ++i) {
                for (size_t j = 0; j < 25; ++j) {
                    result[i] += mind_factor[i][j] * mind_addends[j] + mind_addends[i];
                }
            }

            return result;
        }
};
