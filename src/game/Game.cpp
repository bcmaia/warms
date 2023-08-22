#include "Game.hpp"

Game::Game(unsigned long seed, unsigned population_size) : board() {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned long> dis(0, std::numeric_limits<unsigned long>::max());
    
    // Preallocate the agents vector to the desired size
    agents.reserve(population_size);

    // Generate snakes here using the provided population_size
    for (unsigned i = 0; i < 1 /*population_size*/; ++i) {
        unsigned long snakeSeed = dis(gen);
        agents.emplace_back(Snake(snakeSeed, Position{5, 5}, 3));
    }

    running = true;
}

void Game::process_inputs(int& x, int& y) {
    int ch = getch();

    if (ch == 'q') {
        running = false;
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


void Game::measure_time() {
    using Clock = std::chrono::high_resolution_clock;
    old_frame_time = new_frame_time;
    new_frame_time = Clock::now();
    delta_time = new_frame_time - old_frame_time;
}

void Game::calculate_decisions() {

}

void Game::handle_physics() {
    for (size_t i = 0; i < agents.size(); i++) {
        agents[i].move(board, 1.0);
    }
}

void Game::render_agents() {
    for (size_t i = 0; i < agents.size(); i++) {
        for (size_t j = 0; i < agents[i].body.size(); i++) {
            board.setcell(agents[i].body[j], Cell{'X', agents[i].colorPair});
        }
    }
}

void Game::run() {
    //========================================================================//
    //=================|    INITIALIZATION    |===============================//
    //========================================================================//

    int x = 0, y = 0;

    // Calculate desired frame duration for 100 fps
    constexpr double targetFrameDuration = 1.0 / 100.0; // 100 fps
    std::chrono::duration<double> frameDuration(targetFrameDuration);

    //========================================================================//
    //=================|    GAME LOOP    |====================================//
    //========================================================================//
    while (running) {
        // auto frameStart = Clock::now();

        process_inputs(x, y);

        board.setcell(
            Position{static_cast<short unsigned int>(x), static_cast<short unsigned int>(y)},
            Cell{'*', 1}
        );

        render_agents();
        board.render();

        // auto frameEnd = Clock::now();
        //std::chrono::duration<double> frameTime = frameEnd - frameStart;

        measure_time();

        // Calculate sleep duration to maintain desired frame rate
        std::chrono::duration<double> sleepDuration = frameDuration - delta_time;
        if (sleepDuration > std::chrono::duration<double>(0)) {
            std::this_thread::sleep_for(sleepDuration);
        }
    }
}


