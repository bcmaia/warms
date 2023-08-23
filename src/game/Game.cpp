#include "Game.hpp"

Game::Game(unsigned long seed, unsigned population_size) : board() {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned long> dis(0, std::numeric_limits<unsigned long>::max());
    
    // Preallocate the agents vector to the desired size
    agents.reserve(population_size);

    time_factor = 1;

    //delta_time = 1.0;

    // Generate snakes here using the provided population_size
    for (unsigned i = 0; i < 1 /*population_size*/; ++i) {
        unsigned long snakeSeed = dis(gen);
        agents.emplace_back(Snake(snakeSeed, Position{5, 5}, 7));
    }

    running = true;
}

void Game::process_inputs(int& x, int& y) {
    int ch = getch();

    if ('q' == ch) {
        running = false;
    } else if ('l' == ch) {
        board.toggle_screen_active();
    } else if (ch == KEY_UP && y > 0) {
        y--;
    } else if (ch == KEY_DOWN && y < board.getHeight() - 1) {
        y++;
    } else if (ch == KEY_LEFT && x > 0) {
        x--;
    } else if (ch == KEY_RIGHT && x < board.getWidth() - 1) {
        x++;
    }
}


// void Game::measure_time() {
//     using Clock = std::chrono::high_resolution_clock;
//     old_frame_time = new_frame_time;
//     new_frame_time = Clock::now();
//     delta_time = new_frame_time - old_frame_time;
// }

void Game::calculate_decisions() {

}

void Game::handle_physics(float delta_time) {
    for (size_t i = 0; i < agents.size(); i++) {
        agents[i].move(board, time_factor * delta_time);
    }
}

void Game::render_agents() {
    for (size_t i = 0; i < agents.size(); i++) {
        Snake& snake = agents[i];

        for (const Position& position : snake.body)  {
            board.setcell(position, Cell{'*', snake.colorPair});
        }
        board.setcell(snake.body.front(), Cell{'@', snake.colorPair});
        board.setcell(snake.body.back(), Cell{'+', snake.colorPair});
    }
}

void Game::run() {
    //========================================================================//
    //=================|    INITIALIZATION    |===============================//
    //========================================================================//

    int x = 0, y = 0;

    // Calculate desired frame duration for 100 fps
    constexpr double max_fps = 100;
    constexpr double min_delta_time = 1.0 / max_fps; // 100 fps
    // std::chrono::duration<double> frameDuration(targetFrameDuration);

    //measure_time();
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //measure_time();

    //========================================================================//
    //=================|    GAME LOOP    |====================================//
    //========================================================================//
    while (running) {
        // auto frameStart = Clock::now();

        process_inputs(x, y);

        board.setcell(
            Position{static_cast<short unsigned int>(x), static_cast<short unsigned int>(y)},
            Cell{'X', 1}
        );

        // Simulation
        double delta_time = timer.get_delta_time();
        handle_physics( delta_time );

        // Rendering
        render_agents();
        board.set_delta_time( delta_time );
        board.render();
        //board.displayValue( timer.get_delta_time() * SECONDS_TO_MILISECONDS );

        // auto frameEnd = Clock::now();
        //std::chrono::duration<double> frameTime = frameEnd - frameStart;

        timer.measure_time();
        timer.sync(min_delta_time);

        // // Calculate sleep duration to maintain desired frame rate
        // std::chrono::duration<double> sleepDuration = frameDuration - delta_time;
        // if (sleepDuration > std::chrono::duration<double>(0)) {
        //     std::this_thread::sleep_for(sleepDuration);
        // }
    }
}


