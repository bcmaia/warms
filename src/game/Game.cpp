#include "Game.hpp"

Game::Game(unsigned long seed, unsigned population_size) : board() {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned long> dis(0, std::numeric_limits<unsigned long>::max());
    
    // Preallocate the agents vector to the desired size
    //agents.reserve(population_size);

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

void Game::calculate_decisions() {

}

void Game::handle_physics(float delta_time) {
    for (size_t i = 0; i < agents.size(); i++) {
        agents[i].move(board, 1);
    }
}

void Game::render_agents() {
    for (size_t i = 0; i < agents.size(); i++) {
        Snake& snake = agents[i];

        for (const Position& position : snake.body)  {
            board.setcell(position, Cell{'*', snake.colorPair});
        }
        board.setcell(snake.body.front(), Cell{'@', snake.colorPair});
        board.setcell(snake.body.back(), Cell{'.', snake.colorPair});
    }
}

void Game::run() {
    //========================================================================//
    //=================|    INITIALIZATION    |===============================//
    //========================================================================//

    int x = 0, y = 0;

    // Calculate desired frame duration for 100 fps
    const double targetFrameDuration = 1.0 / 1.0; // 100 fps

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
            Cell{'*', 1}
        );

        render_agents();
        handle_physics(1);
        board.render();
        board.displayValue(agents[0].body.size());

        // auto frameEnd = Clock::now();
        //std::chrono::duration<double> frameTime = frameEnd - frameStart;

        double sleep_time = targetFrameDuration - timer.get_delta_time();

        if (0.001 < sleep_time) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
        }
    }
}


