#include "Snake.hpp"


Snake::Snake(unsigned long seed, Position initial = Position{0, 0}, unsigned short start_lenght = 3) : genes(GENE_SIZE), gen(seed) {
    // Initialize genes with random values
    std::uniform_real_distribution<float> dis(-1000.0, 1000.0);

    for (size_t i = 0; i < GENE_SIZE; ++i) {
        genes[i] = dis(gen);
    }

    alive = true;
    facing = Direction::Up;

    //body.reserve(8);
    lenght = start_lenght;
    colorPair = 1;
    body.emplace_back(initial);

    speed = 1;
    movement = 0;

    // Initialize other member variables if needed
    // Example: initialize body, length, facing, etc.
}

void Snake::die () {
    alive = false;
}

Position Snake::check_colision(Board& board) {
    Position new_point = board.movement(body.front(), facing, 1);

    if (board.isSolidAt(new_point)) {
        die();
    }

    return new_point;
}

void Snake::move(Board& board, float deltaTime) {
    movement += speed * deltaTime;
    if (1 > movement) return;
    movement--;

    Position new_point = check_colision(board);

    if (!alive) return;

    if (lenght < body.size() )  {
        body.push_front(new_point);
    } else if (lenght == body.size() ) {
        body.push_front(new_point);
        (void) body.pop_back();
    } else /* if (lenght > body.size()) */ {
        (void) body.pop_back();
    }
}