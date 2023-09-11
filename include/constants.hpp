#pragma once

#define POPULATION_SIZE (64)
#define GOATS_VEC_SIZE (16)
#define STOCK_VEC_SIZE (256)
#define MAKE_SIMULATION_GREAT_AGAIN (false)

#define MIN_SPEED (1.0)
#define MAX_SPEED (4.0)

#define STARTING_ENERGY (5.0)

#define RANDOM_WALLS_CHANCE_MIN (1.0 / 131.0)
#define RANDOM_WALLS_CHANCE_MAX (1.0 / 2.3)

#define CLEANER_TIMER (6 * 1000)

// SNAKE FACTORS
// SHRINK_FACTOR : Increase it and snakes will shrink faster.
#define SHRINK_FACTOR (1.0 / 5000.0)

// MUTATION RATES
#define MUTATION_RATE (0.001)
#define MUTATION_RATE_2 (0.02)
#define COLOR_MUTATION_RATE (0.02)

// NEURAL NETWORK SIZE
// NOTE: changing this values can break stuff
#define OUTPUT_SIZE (2)
#define INPUT_SIZE (11 * 11 * 6 + 6)
#define HIDDEN_SIZE (63)

#define SOFT_GENOCIDE_INTERVAL (20.0 * 60.0 * 1000.0)
#define FITNESS_LOG_FILE_NAME ("fitness_test_21.csv")