#pragma once

#include <vector>

#include "utils.hpp"
#include "types.hpp"


#include <vector>
#include <random>
#include <stdexcept>



#define MIND_SIZE (10)


class Genome {
    public:
        matrixf32 mind_factor;
        vectorf32 mind_addends;
        ColorPair colorPair;

        ~Genome(){};

        Genome(unsigned long seed) {
            // Constructor to initialize the matrix and vector
            mind_factor = generateRandomMatrix(3, MIND_SIZE, seed);
            mind_addends = generateRandomVector(3, seed);

            colorPair = 10;
        }

        void mutate(float mutationProbability, unsigned long seed) {
            std::mt19937 gen(seed);
            std::uniform_real_distribution<float> dis2(-3.0, 3.0);
            std::uniform_real_distribution<float> dis(-1000.0, 1000.0);
            std::bernoulli_distribution mutateDist(mutationProbability);
            std::bernoulli_distribution mutationTypeDist(0.5);

            for (size_t i = 0; i < 3; ++i) {
                for (size_t j = 0; j < MIND_SIZE; ++j) {
                    if (mutateDist(gen)) {
                        if (mutationTypeDist(gen))
                            mind_factor[i][j] = dis(gen);
                        else
                            mind_factor[i][j] *= dis2(gen);
                    }
                }
                if (mutateDist(gen)) {
                    if (mutationTypeDist(gen))
                        mind_addends[i] = dis(gen);
                    else
                        mind_addends[i] *= dis2(gen);
                }

                if (mutateDist(gen)) {
                    colorPair = static_cast<unsigned>(16 + 16 * dis(gen)) % 16;
                }
            }
        }

        Genome(const Genome& parent1, const Genome& parent2, unsigned long seed) {
            // Constructor for crossover between two Genome
            std::mt19937 gen(seed);
            std::uniform_int_distribution<size_t> dis(0, 1); // 0 or 1 randomly

            mind_factor = matrixf32(3, vectorf32(MIND_SIZE, 0.0));
            mind_addends = vectorf32(3, 0.0);

            for (size_t i = 0; i < 3; ++i) {
                for (size_t j = 0; j < MIND_SIZE; ++j) {
                    mind_factor[i][j] = (1 == dis(gen)) ? parent1.mind_factor[i][j] : parent2.mind_factor[i][j];
                }
                mind_addends[i] = (1 == dis(gen)) ? parent1.mind_addends[i] : parent2.mind_addends[i]; 
            }

            colorPair = (1 == dis(gen)) ? parent1.colorPair : parent2.colorPair;

            mutate(0.035, seed);
        }

        vectorf32 think (const vectorf32& sensorial_input) {
            vectorf32 result(3);
            
            #ifndef RELEASE
                if (
                    mind_factor.size() != 3 
                    || mind_factor[0].size() != MIND_SIZE
                    || sensorial_input.size() != MIND_SIZE 
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
                for (size_t j = 0; j < MIND_SIZE; ++j) {
                    result[i] += mind_factor[i][j] * sensorial_input[j] + mind_addends[i];
                }
            }

            return result;
        }
};
