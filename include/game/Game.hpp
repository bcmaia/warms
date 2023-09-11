#pragma once

#include <vector>
#include <algorithm>
#include <execution>
#include <fstream>

#include "constants.hpp"
#include "types.hpp"
#include "Snake.hpp"
#include "Timer.hpp"

using chrono = std::chrono::time_point<std::chrono::high_resolution_clock>;

class Game {
    public:
        Game(unsigned long seed, unsigned population_size);
        ~Game(){};
        void run();
        
    private:
        std::vector<SavedGenome> bests;
        std::vector<SavedGenome> stock;
        std::mt19937 gen;

        Board board;
        int population_size;
        void render_agents_setup();

        float time_factor;
        bool running;
        Timer timer;

        
        std::vector<Snake> agents;
        

        void measure_time();
        void process_inputs(int& x, int& y, char& type, bool& fast);
        void reproduce();
        void handle_physics(float delta_time);
        void render_agents();
        void process_thought();
        void run_simulation(float delta_time);
        void updateBestItem(const SavedGenome& newItem);
        SavedGenome getRandomGenome();
        Genome get_random_parent ();
        void soft_genocide ();
};