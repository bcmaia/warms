#pragma once

#include "constants.hpp"
#include "utils.hpp"
#include "types.hpp"


#include <vector>
#include <random>
#include <stdexcept>
#include <cmath>



class Genome {
    public:
        float speed;
        matrixf32 input_to_hidden_weights;
        vectorf32 hidden_biases;
        matrixf32 hidden_to_output_weights;
        vectorf32 output_biases;
        ColorPair colorPair;

        float activation_function(float x) {
            return tanh(x);
        }

        ~Genome() {};

        Genome() : Genome (std::rand()) {};

        Genome(unsigned long seed) {
            // Constructor to initialize weights and biases
            input_to_hidden_weights = generateRandomMatrix(HIDDEN_SIZE, INPUT_SIZE, seed);
            hidden_biases = generateRandomVector(HIDDEN_SIZE, seed);
            hidden_to_output_weights = generateRandomMatrix(OUTPUT_SIZE, HIDDEN_SIZE, seed);
            output_biases = generateRandomVector(OUTPUT_SIZE, seed);

            speed = MIN_SPEED;
            colorPair = 10;
        }

        Genome(const Genome& parent1, const Genome& parent2, unsigned long seed) {
            // Constructor for crossover between two Genomes
            std::mt19937 gen(seed);
            std::uniform_real_distribution<float> dis2(-3.0, 3.0);
            std::uniform_real_distribution<float> dis(-1000.0, 1000.0);
            std::bernoulli_distribution mutateDist(MUTATION_RATE);
            std::bernoulli_distribution mutationTypeDist(0.5);

            input_to_hidden_weights = matrixf32(HIDDEN_SIZE, vectorf32(INPUT_SIZE, 0.0));
            hidden_biases = vectorf32(HIDDEN_SIZE, 0.0);
            hidden_to_output_weights = matrixf32(OUTPUT_SIZE, vectorf32(HIDDEN_SIZE, 0.0));
            output_biases = vectorf32(OUTPUT_SIZE, 0.0);

            speed = (1 == dis(gen)) ? parent1.speed : parent2.speed;

            for (size_t i = 0; i < HIDDEN_SIZE; ++i) {
                for (size_t j = 0; j < INPUT_SIZE; ++j) {
                    input_to_hidden_weights[i][j] = (1 == dis(gen)) ? parent1.input_to_hidden_weights[i][j] : parent2.input_to_hidden_weights[i][j];
                }
                hidden_biases[i] = (1 == dis(gen)) ? parent1.hidden_biases[i] : parent2.hidden_biases[i];
            }

            for (size_t i = 0; i < OUTPUT_SIZE; ++i) {
                for (size_t j = 0; j < HIDDEN_SIZE; ++j) {
                    hidden_to_output_weights[i][j] = (1 == dis(gen)) ? parent1.hidden_to_output_weights[i][j] : parent2.hidden_to_output_weights[i][j];
                }
                output_biases[i] = (1 == dis(gen)) ? parent1.output_biases[i] : parent2.output_biases[i];
            }

            colorPair = (1 == dis(gen)) ? parent1.colorPair : parent2.colorPair;

            mutate(MUTATION_RATE, seed);
        }

    void mutate(float mutationProbability, unsigned long seed) {
        std::mt19937 gen(seed);

        std::uniform_real_distribution<float> dis2(-2.0, 2.0);
        std::uniform_real_distribution<float> dis(-1.0, 1.0);

        std::bernoulli_distribution mutateDist(mutationProbability);
        std::bernoulli_distribution mutationTypeDist(0.5);

        if (mutateDist(gen)) {
            if (mutationTypeDist(gen))
                speed = dis(gen);
            else
                speed *= dis2(gen);

            speed = 
                MIN_SPEED > speed ? MIN_SPEED :
                MAX_SPEED < speed ? MAX_SPEED :
                speed;
        }

        for (size_t i = 0; i < HIDDEN_SIZE; ++i) {
            for (size_t j = 0; j < INPUT_SIZE; ++j) {
                if (mutateDist(gen)) {
                    if (mutationTypeDist(gen))
                        input_to_hidden_weights[i][j] = dis(gen);
                    else
                        input_to_hidden_weights[i][j] *= dis2(gen);
                }
            }
            if (mutateDist(gen)) {
                if (mutationTypeDist(gen))
                    hidden_biases[i] = dis(gen);
                else
                    hidden_biases[i] *= dis2(gen);
            }
        }

        for (size_t i = 0; i < OUTPUT_SIZE; ++i) {
            for (size_t j = 0; j < HIDDEN_SIZE; ++j) {
                if (mutateDist(gen)) {
                    if (mutationTypeDist(gen))
                        hidden_to_output_weights[i][j] = dis(gen);
                    else
                        hidden_to_output_weights[i][j] *= dis2(gen);
                }
            }
            if (mutateDist(gen)) {
                if (mutationTypeDist(gen))
                    output_biases[i] = dis(gen);
                else
                    output_biases[i] *= dis2(gen);
            }
        }

        std::bernoulli_distribution colorMutateDist(COLOR_MUTATION_RATE);
        if (colorMutateDist(gen)) {
            colorPair = static_cast<unsigned>(16 + 16 * dis(gen)) % 16;
        }
    }

        vectorf32 think(const vectorf32& sensorial_input) {
            vectorf32 hidden_layer(HIDDEN_SIZE);
            vectorf32 result(OUTPUT_SIZE);

            #ifndef RELEASE
            if (
                input_to_hidden_weights.size() != HIDDEN_SIZE || input_to_hidden_weights[0].size() != INPUT_SIZE ||
                hidden_to_output_weights.size() != OUTPUT_SIZE || hidden_to_output_weights[0].size() != HIDDEN_SIZE ||
                sensorial_input.size() != INPUT_SIZE ||
                hidden_biases.size() != HIDDEN_SIZE || output_biases.size() != OUTPUT_SIZE
            ) {
                throw std::runtime_error(
                    "Sizes don't match: input_to_hidden_weights[" + std::to_string(input_to_hidden_weights.size()) +
                    "][" + std::to_string(input_to_hidden_weights[0].size()) +
                    "], sensorial_input[" + std::to_string(sensorial_input.size()) +
                    "], hidden_biases[" + std::to_string(hidden_biases.size()) +
                    "], hidden_to_output_weights[" + std::to_string(hidden_to_output_weights.size()) +
                    "][" + std::to_string(hidden_to_output_weights[0].size()) + "], output_biases[" +
                    std::to_string(output_biases.size()) + "]"
                );
            }
            #endif

            // Calculate the output of the hidden layer
            for (size_t i = 0; i < HIDDEN_SIZE; ++i) {
                hidden_layer[i] = 0.0;
                for (size_t j = 0; j < INPUT_SIZE; ++j) {
                    hidden_layer[i] += input_to_hidden_weights[i][j] * sensorial_input[j];
                }
                hidden_layer[i] += hidden_biases[i];
                hidden_layer[i] = activation_function(hidden_layer[i]);
            }

            // Calculate the final result (output layer)
            for (size_t i = 0; i < OUTPUT_SIZE; ++i) {
                result[i] = 0.0;
                for (size_t j = 0; j < HIDDEN_SIZE; ++j) {
                    result[i] += hidden_to_output_weights[i][j] * hidden_layer[j];
                }
                result[i] += output_biases[i];
            }

            return result;
        }


        Genome(const Genome& other) {
            // Copy constructor for deep copy
            input_to_hidden_weights = other.input_to_hidden_weights;
            hidden_biases = other.hidden_biases;
            hidden_to_output_weights = other.hidden_to_output_weights;
            output_biases = other.output_biases;
            colorPair = other.colorPair;
        }

        Genome& operator=(const Genome& other) {
            // Copy assignment operator for deep copy
            if (this != &other) {
                input_to_hidden_weights = other.input_to_hidden_weights;
                hidden_biases = other.hidden_biases;
                hidden_to_output_weights = other.hidden_to_output_weights;
                output_biases = other.output_biases;
                colorPair = other.colorPair;
            }
            return *this;
        }
};


class SavedGenome {
    public:
        float fitness;
        Genome genome;

        SavedGenome (float saved_fitness, const Genome& saved_genome) 
            : fitness(saved_fitness), genome(saved_genome) {}

        bool operator> (const SavedGenome& other) const {
            return fitness > other.fitness;
        }
};
