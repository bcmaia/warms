#pragma once

#include <vector>

#include "types.hpp"
#include "Snake.hpp"

using chrono = std::chrono::time_point<std::chrono::high_resolution_clock>;

class Game {
    public:
        Game(unsigned long seed, unsigned population_size);
        ~Game(){};
        void run();
        
    private:
        float time_factor;
        bool running;
        chrono old_frame_time;
        chrono new_frame_time;
        std::chrono::duration<double> delta_time;

        Board board;
        std::vector<Snake> agents;
        std::mt19937 gen;

        void measure_time();
        void process_inputs(int& x, int& y);
        void calculate_decisions();
        void handle_physics(float delta_time);
        void render_agents();
};