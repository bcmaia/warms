#include "Snake.hpp"



Snake::Snake(
    unsigned long seed, 
    Position initial = Position{0, 0}, 
    unsigned short start_lenght = 10
) : genome(seed), gen(seed) {
    // Initialize genes with random values
    std::uniform_real_distribution<float> dis(-1000.0, 1000.0);
    //std::uniform_int_distribution<int> dis(-1000, 1000.0);

    // for (size_t i = 0; i < GENE_SIZE; ++i) {
    //     genes[i] = dis(gen);
    // }

    time_alive = 0;
    moved = false;
    alive = true;
    float r = dis(gen);
    facing = static_cast<Direction>(static_cast<unsigned>(r) % 4);

    //body.reserve(8);
    state = 0;
    fitness = 0;
    lenght = start_lenght;
    r = dis(gen);
    colorPair = 1 + static_cast<unsigned>(r) % 14;
    body.emplace_back(initial);

    speed = 0.01;
    movement = 0;

    shrink = 0;

    // Initialize other member variables if needed
    // Example: initialize body, length, facing, etc.
}







Snake::Snake(
    Genome& parent1,
    Genome& parent2,
    unsigned long seed, 
    Position initial = Position{0, 0}, 
    unsigned short start_lenght = 10
) : genome(parent1, parent2, seed), gen(seed) {
    // Initialize genes with random values
    std::uniform_real_distribution<float> dis(-1000.0, 1000.0);
    // std::uniform_real_distribution<float> dis(-1000, 1000.0);

    // for (size_t i = 0; i < GENE_SIZE; ++i) {
    //     genes[i] = dis(gen);
    // }

    time_alive = 0;
    moved = false;
    alive = true;
    float r = dis(gen);
    facing = static_cast<Direction>(static_cast<unsigned>(r) % 4);

    //body.reserve(8);
    lenght = start_lenght;
    r = dis(gen);
    colorPair = 1 + static_cast<unsigned>(r) % 14;
    body.emplace_back(initial);

    speed = 0.01;
    movement = 0;

    shrink = 0;

    // Initialize other member variables if needed
    // Example: initialize body, length, facing, etc.
}








void Snake::die (Board& board) {
    alive = false;

    fitness -= 3.0;
    if (0 < fitness) fitness *= 0.5;

    for (const Position &p : body) {
        board.setcell(p, Cell{'&', 1});
    }
}

void Snake::think(Board& board) {
    vectorf32 sensorial_input = board.get_sensorial_data(body.front());
    
    std::uniform_real_distribution<float> dis(-2.0, 2.0);
    // endwin();
    // printf("< sensorial input size [%ld] >", sensorial_input.size());
    // exit(1);

    sensorial_input.push_back( ((float)lenght) * (1.0 / 64.0) );
    sensorial_input.push_back( dis(gen) );
    sensorial_input.push_back( state );
    sensorial_input.push_back( time_alive );
    sensorial_input.push_back( static_cast<float>(facing) * (1.0 / 3.0) );
    sensorial_input.push_back( static_cast<float>(oldFacing) * (1.0 / 3.0) );

    vectorf32 decision_vec = genome.think(sensorial_input);
    state = decision_vec[0];
    float new_direction = decision_vec[1];

    if (-0.5 > new_direction) facing = rotate(facing, false);
    else if (0.5 < new_direction) facing = rotate(facing, true);
    
    // int biggest = 0;

    // for (size_t i = 0; i < 3; i++) {
    //     if (decision_vec[i] > decision_vec[biggest]) biggest = i;
    // }

    if (oldFacing != facing) fitness -= 0.01;

    oldFacing = facing;

    // switch (biggest) {
    //     case 1: 
    //         facing = rotate(facing, false);
    //         break;
    //     // case 1:
    //     //     break;
    //     case 2: 
    //         facing = rotate(facing, true);
    //         break;
    // }
    //facing = ((int)(rand() % 5)) != 1 ? Direction::Left : Direction::Up;

    //if (oldFacing != facing) fitness += 1.0;
}

void Snake::move(Board& board, float deltaTime) {
    // If we are alive
    if (!alive) return;

    // If we must shrink
    shrink += SHRINK_FACTOR * deltaTime;
    if (1.0 < shrink) {
        lenght--;
        shrink--;
        if (3 > lenght) {die(board); return;}
    } 

    // If we have momentum
    movement += speed * deltaTime;
    if (1.0 > movement) return;
    movement--;

    // Calculate new head
    neck = body.front();
    Position new_point = board.movement(body.front(), facing, 1);

    // Check for colision
    if (board.isSolidAt(new_point)) {
        die(board); 
        return;
    }

    // Check for berries
    if (board.compare(new_point, '&') || board.compare(new_point, '6')) {
        lenght++;
        fitness += 1.0;
    }

    fitness += deltaTime * 0.01;
    

    // Add new cell
    body.push_front(new_point);
    new_cell.set_value(new_point);

    // Remove tail
    if (body.size() > lenght) {
        dead_cell.set_value(body.back());
        body.pop_back();
    }

    // Remove second
    if (body.size() > lenght) {
        dead_cell_2.set_value(body.back());
        body.pop_back();
    }

    time_alive += deltaTime;
}


void Snake::shed_dead_cell (Board& board) {
    if (dead_cell.has_value()) {
        board.setcell(dead_cell.unwrap(), Cell(' ', 0));
        board.setcell(body.back(), Cell('+', genome.colorPair));
        dead_cell.clear();
    }

    if (dead_cell_2.has_value()) {
        board.setcell(dead_cell_2.unwrap(), Cell(' ', 0));
        dead_cell_2.clear();
    }
}

void Snake::show_new_cell (Board& board) {
    if (new_cell.has_value()) {
        board.setcell(neck, Cell{'*', genome.colorPair});
        board.setcell(new_cell.unwrap(), Cell{'@', genome.colorPair});
        new_cell.clear();
    }
}

void Snake::render (Board& board) {
    for (const Position &p : body) {
        board.setcell(p, Cell{'*', genome.colorPair});
    }

    if (0 < body.size()) {
        board.setcell(body.back(), Cell{'+', genome.colorPair});
        board.setcell(body.front(), Cell{'@', genome.colorPair});
    }
}

void Snake::survival_test(Board& board) {
    if (!moved) return;

    // If the board is solid at where we are going, we die.
    alive = !board.isSolidAt(body.front());

    // TODO: Handle death sequence
    //...
}

bool compare_snakes (const Snake& s1, const Snake& s2) {
    return s1.time_alive < s2.time_alive;
}

