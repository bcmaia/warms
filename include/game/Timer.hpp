#include <chrono>

constexpr double SECONDS_TO_MILISECONDS = 1000.0;
constexpr double MIN_DELTA_TIME = 0.001; // In seconds


class Timer {
    private:
        std::chrono::high_resolution_clock::time_point old_time;
        std::chrono::high_resolution_clock::time_point new_time;
        double delta_time;

    public:
        Timer() {
            old_time = std::chrono::high_resolution_clock::now();
            new_time = old_time;
            delta_time = 1.0; // Starts at one second
        }

        void measure_time() {
            old_time = new_time;
            new_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = new_time - old_time;
            delta_time = duration.count();
            if (MIN_DELTA_TIME > delta_time) delta_time = MIN_DELTA_TIME;
        }

        double get_delta_time() const {
            return delta_time;
        }

        void sync (double min_delta_time) const {
            double sleep_time = min_delta_time - delta_time;
            if (MIN_DELTA_TIME >= sleep_time) return;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
        }
};
