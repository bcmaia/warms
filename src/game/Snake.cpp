#include "Snake.hpp"


Snake::Snake(unsigned long seed, Position initial = Position{0, 0}, unsigned short start_lenght = 3) : genes(GENE_SIZE), gen(seed) {
    // Initialize genes with random values
    std::uniform_real_distribution<float> dis(-1000.0, 1000.0);

    for (size_t i = 0; i < GENE_SIZE; ++i) {
        genes[i] = dis(gen);
    }

    alive = true;

    oldFacing = Direction::Right;
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

void Snake::think (Board& board) {
    oldFacing = facing;
    facing = ((int)(rand() % 5)) != 1 ? Direction::Left : Direction::Up;
}

void Snake::move(Board& board, float delta_time) {
    movement += speed * delta_time;
    if (1.0 > movement) return;
    movement--;

    if (!alive) return;
    
    unsigned u_movement = static_cast<unsigned>(movement);
    Position new_point = board.movement(body.back(), facing, u_movement);

    newCells.push_back(body.back());
    newCells.push_back(new_point);

    if (oldFacing == facing) {
        body.back() = new_point;
    } else {
        body.push_back(new_point);
    }

    deadCells.push_back(body.front());
    for (int x = u_movement ;;) {
        if (2 > body.size()) {
            die();
            break;
        }

        Position delta = body[1] - body[0];
        Position dif = delta.trim(x);

        x -= dif.manhattan_magnitude();

        Position next_point = body[0] + dif;

        if (body[1] == next_point) {
            body.pop_front();
            deadCells.push_back(body.front());
        } else {
            body[0] = next_point;
            deadCells.push_back(delta.trim(x - 1));
            break;
        }
    }
}




void Snake::shed_dead_cells (Board& board) {
    //if (0 == deadCells.size()) return;

    for (auto it = deadCells.begin(); std::next(it) != deadCells.end(); ++it) {
        Position currentElement = *it;
        Position nextElement = *(std::next(it));
        board.set_line(currentElement, nextElement, Cell{' ', 0});
    }

    deadCells.clear();
}

void Snake::show_new_cells (Board& board) {
    //if (0 == newCells.size()) return;

    for (auto it = newCells.begin(); std::next(it) != newCells.end(); ++it) {
        Position currentElement = *it;
        Position nextElement = *(std::next(it));
        board.set_line(currentElement, nextElement, Cell{'*', colorPair});
    }

    board.setcell(body.front(), Cell{'+', colorPair});
    board.setcell(body.back(), Cell{'@', colorPair});

    newCells.clear();
}