#pragma once

#include <vector>

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
        int population_size;
        void render_agents_setup();

        float time_factor;
        bool running;
        Timer timer;

        Board board;
        std::vector<Snake> agents;
        std::mt19937 gen;

        void measure_time();
        void process_inputs(int& x, int& y, char& type, bool& fast);
        void reproduce();
        void handle_physics(float delta_time);
        void render_agents();
        void process_thought();
};