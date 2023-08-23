#include "Snake.hpp"


Snake::Snake(unsigned long seed, Position initial = Position{0, 0}, unsigned short start_lenght = 3) : genes(GENE_SIZE), gen(seed) {
    // Initialize genes with random values
    std::uniform_real_distribution<float> dis(-1000.0, 1000.0);

    for (size_t i = 0; i < GENE_SIZE; ++i) {
        genes[i] = dis(gen);
    }

    moved = false;
    alive = true;
    facing = Direction::Right;

    //body.reserve(8);
    lenght = start_lenght;
    colorPair = 1;
    body.emplace_back(initial);

    speed = 0.01;
    movement = 0;

    // Initialize other member variables if needed
    // Example: initialize body, length, facing, etc.
}

void Snake::die () {
    alive = false;
}

void Snake::think(Board& board) {
    facing = ((int)(rand() % 5)) != 1 ? Direction::Left : Direction::Up;
}

void Snake::move(const Board& board, float deltaTime) {
    // If we are alive
    if (!alive) return;

    // If we have momentum
    movement += speed * deltaTime;
    if (1.0 > movement) return;
    movement--;

    // Calculate new head
    neck = body.front();
    Position new_point = board.movement(body.front(), facing, 1);
    

    // Check for colision
    // alive = board.isSolidAt(new_point);
    // if (!alive) return;

    // Add new cell
    body.push_front(new_point);
    new_cell.set_value(new_point);

    // Remove tail
    if (body.size() > lenght) {
        dead_cell.set_value(body.back());
        body.pop_back();
    }
}

constexpr Cell DEAD_CELL = Cell{' ', 0};

void Snake::shed_dead_cell (Board& board) {
    if (dead_cell.has_value()) {
        board.setcell(dead_cell.unwrap(), DEAD_CELL);
        board.setcell(body.back(), Cell{'+', colorPair});
        dead_cell.clear();
    }
}

void Snake::show_new_cell (Board& board) {
    if (new_cell.has_value()) {
        board.setcell(neck, Cell{'*', colorPair});
        board.setcell(new_cell.unwrap(), Cell{'@', colorPair});
        new_cell.clear();
    }
}

void Snake::render (Board& board) {
    for (const Position &p : body) {
        board.setcell(p, Cell{'*', colorPair});
    }

    if (0 < body.size()) {
        board.setcell(body.back(), Cell{'+', colorPair});
        board.setcell(body.front(), Cell{'@', colorPair});
    }
}

void Snake::survival_test(Board& board) {
    if (!moved) return;

    // If the board is solid at where we are going, we die.
    alive = !board.isSolidAt(body.front());

    // TODO: Handle death sequence
    //...
}

