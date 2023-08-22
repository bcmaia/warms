#pragma once

#include <chrono>

class Timer {
    private:
        std::chrono::high_resolution_clock::time_point old_time;
        std::chrono::high_resolution_clock::time_point new_time;
        double delta_time;

    public:
        Timer() {
            old_time = std::chrono::high_resolution_clock::now();
            new_time = old_time;
            delta_time = 1.0f;
        }

        void measure_time() {
            old_time = new_time;
            new_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = new_time - old_time;
            delta_time = duration.count();
            if (0 >= delta_time) {delta_time = 0.001;}
        }

        double get_delta_time() const {
            return delta_time;
        }
};