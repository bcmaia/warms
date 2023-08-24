#include "Game.hpp"

Game::Game(unsigned long seed, unsigned population_size) : gen(seed), board(), population_size(population_size) {
    //std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned long> dis(0, std::numeric_limits<unsigned long>::max());
    
    // Preallocate the agents vector to the desired size
    agents.reserve(population_size + 1);

    time_factor = 1;

    //delta_time = 1.0;

    // Generate snakes here using the provided population_size
    for (unsigned i = 0; i < population_size; ++i) {
        unsigned long snakeSeed = dis(gen);

        //ColorPair color = 1 + dis(gen) % 10;

        Position p = board.rand_empty_position(snakeSeed);
        agents.push_back(Snake(snakeSeed, p, 7));
    }

    running = true;
}

void Game::process_inputs(int& x, int& y, char& type, bool& fast) {
    int ch = getch();

    if ('q' == ch) {
        running = false;
    } else if ('l' == ch) {
        board.toggle_screen_active();
        fast = !fast;
    } else if ('\n' == ch) {
        board.set_cursor(Position(x, y), type, true);
    } else if ('\t' == ch) {
        type = '#' == type ? 'X' : '#';
    } else if (ch == KEY_UP && y > 0) {
        y--;
    } else if (ch == KEY_DOWN && y < board.getHeight() - 1) {
        y++;
    } else if (ch == KEY_LEFT && x > 0) {
        x--;
    } else if (ch == KEY_RIGHT && x < board.getWidth() - 1) {
        x++;
    }

    board.set_cursor(Position(x, y), type, false);
}


// void Game::measure_time() {
//     using Clock = std::chrono::high_resolution_clock;
//     old_frame_time = new_frame_time;
//     new_frame_time = Clock::now();
//     delta_time = new_frame_time - old_frame_time;
// }

void Game::reproduce() {
    //return;
    std::uniform_int_distribution<std::size_t> index_dist (0, agents.size() - 1);
    std::uniform_int_distribution<unsigned long> seed_dist (0, std::numeric_limits<unsigned long>::max());
    //return;
    std::vector<Snake> new_snakes;

    // the 2 fors keep things fair
    for (size_t i = 0; i < agents.size(); i++) {
        Snake& snake = agents[i];
        if (!snake.alive) {
            //continue;
            std::size_t index1 = index_dist(gen);
            std::size_t index2 = index_dist(gen);

            Snake& parent1 = (
                agents[index1].time_alive + agents[index1].lenght > agents[index2].time_alive + agents[index2].lenght
                ? agents[index1] 
                : agents[index2]
            );

            index1 = index_dist(gen);
            index2 = index_dist(gen);

            Snake& parent2 = (
                agents[index1].time_alive + agents[index1].lenght > agents[index2].time_alive + agents[index2].lenght
                ? agents[index1] 
                : agents[index2]
            );

            // index1 = index_dist(gen);
            // index2 = index_dist(gen);
            // Snake& competitor1 = agents[index1];
            // Snake& competitor2 = agents[index2];
            // Snake& parent1 =  agents[index1].time_alive > agents[index2].time_alive ? agents[index1] : agents[index2];

            // index1 = index_dist(gen) % agents.size();
            // index2 = index_dist(gen) % agents.size();
            // Snake& parent2 = agents[index1].time_alive > agents[index2].time_alive ? agents[index1] : agents[index2];

            unsigned long snakeSeed = seed_dist(gen);
            Position p = board.rand_empty_position(snakeSeed);
            agents.erase(agents.begin() + i);
            i--;
            new_snakes.push_back( Snake(parent1.genome, parent2.genome, snakeSeed, p, 7) );
        }

        while (0 < new_snakes.size()) {
            agents.push_back(new_snakes.back());
            new_snakes.pop_back();
        }
    }
}


void Game::handle_physics(float delta_time) {
    for (size_t i = 0; i < agents.size(); i++) {
        agents[i].move(board, time_factor * delta_time);
    }
}

void Game::process_thought() {
    for (size_t i = 0; i < agents.size(); i++) {
        Snake& snake = agents[i];
        snake.think(board);
    }
}

void Game::render_agents() {
    for (size_t i = 0; i < agents.size(); i++) {
        Snake& snake = agents[i];
        snake.shed_dead_cell(board);
    }

    for (size_t i = 0; i < agents.size(); i++) {
        Snake& snake = agents[i];
        snake.show_new_cell(board);
    }

}

void Game::render_agents_setup() {
    for (size_t i = 0; i < agents.size(); i++) {
        Snake& snake = agents[i];
        snake.render(board);
    }

}


void Game::run() {
    //========================================================================//
    //=================|    INITIALIZATION    |===============================//
    //========================================================================//

    int x = 0, y = 0;
    char type = 'X';
    bool fast = false;

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
    int count = 0;

    constexpr double cleaner_factor = 10000.0;
    double cleaner_timer = 0;

    while (running) {
        // auto frameStart = Clock::now();

        
        process_inputs(x, y, type, fast);

        // board.pri(
        //     Position{static_cast<short unsigned int>(x), static_cast<short unsigned int>(y)},
        //     Cell{'X', 1}
        // );

        // Simulation
        double delta_time = fast ? min_delta_time : timer.get_delta_time();

        cleaner_timer += delta_time;
        if (cleaner_factor < cleaner_timer) {
            cleaner_timer = 0;
            board.rot_all();
        }

        handle_physics( delta_time );

        // Rendering
        
        /*if (10000 > count++) render_agents_setup();
        else */
        process_thought();
        render_agents();
        board.set_delta_time( timer.get_delta_time() );
        board.render();
        //board.displayValue( timer.get_delta_time() * SECONDS_TO_MILISECONDS );

        // auto frameEnd = Clock::now();
        //std::chrono::duration<double> frameTime = frameEnd - frameStart;

        timer.measure_time();
        if (!fast) {timer.sync(min_delta_time);}

        reproduce();

        // // Calculate sleep duration to maintain desired frame rate
        // std::chrono::duration<double> sleepDuration = frameDuration - delta_time;
        // if (sleepDuration > std::chrono::duration<double>(0)) {
        //     std::this_thread::sleep_for(sleepDuration);
        // }
    }
}


