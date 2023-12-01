#pragma once

#define POPULATION_SIZE (64)
#define GOATS_VEC_SIZE (16)
#define STOCK_VEC_SIZE (256)
#define MAKE_SIMULATION_GREAT_AGAIN (true)

#define MIN_SPEED (1.0)
#define MAX_SPEED (4.0)

#define STARTING_ENERGY (5.0)

#define RANDOM_WALLS_CHANCE_MIN (0.0 / 131.0)
#define RANDOM_WALLS_CHANCE_MAX (0.0 / 2.3)

#define CLEANER_TIMER (5 * 1000)

// SNAKE FACTORS
// SHRINK_FACTOR : Increase it and snakes will shrink faster.
#define SHRINK_FACTOR (1.0 / 5000.0)

// MUTATION RATES
#define MUTATION_RATE (0.02)

// NEURAL NETWORK SIZE
// NOTE: changing this values can break stuff
#define OUTPUT_SIZE (3)
#define INPUT_SIZE (7 * 7 * 4 + 8 * 4)
#define HIDDEN_SIZE (63)

#define SOFT_GENOCIDE_INTERVAL (20.0 * 60.0 * 1000.0)
#define FITNESS_LOG_FILE_NAME ("fitness.csv")