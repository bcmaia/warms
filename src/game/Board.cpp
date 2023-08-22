#include "Board.hpp"

Board::Board() {
    setenv("LANG", "en_US.UTF-8", 1);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);
    width = maxX;
    height = maxY;

    matrix = std::make_unique<std::vector<std::vector<Cell>>>(
        height, std::vector<Cell>(width, Cell{' ', 1})
    );

    auxiliar = std::make_unique<std::vector<std::vector<Cell>>>(
        height, std::vector<Cell>(width, Cell{' ', 1})
    );
}

Board::~Board() {
    width = 0;
    height = 0;

    matrix = NULL;
    auxiliar = NULL;

    endwin();
}

// void Board::printat (Position point, Cell cell) {
//     (*matrix)[point.y][point.x] = cell;
//     mvaddch(point.y, point.x, cell.character | COLOR_PAIR(cell.colorPair));
// }

void Board::setcell (Position point, Cell cell) {
    (*auxiliar)[point.y][point.x] = cell;
}

void Board::render_static () const {
    for (size_t y = 0; y < matrix->size(); ++y) {
        for (size_t x = 0; x < (*matrix)[y].size(); ++x) {
            const Cell& cell = (*matrix)[y][x];
            mvaddch(y, x, cell.character | COLOR_PAIR(cell.colorPair));
        }
    }
    refresh();
}

void Board::render () {
    render_static ();
    swap();
    reset();
}

void Board::reset () {
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            (*auxiliar)[i][j] = Cell{' ', 0};
        }
    }
}

Position Board::movement (Position point, Direction dir, short amount = 1) const {
    switch (dir) {
        case Direction::Up:
            return Position{point.x, static_cast<unsigned short>((point.y + amount) % height)};
        case Direction::Down:
            return Position{point.x, static_cast<unsigned short>((point.y - amount) % height)};
        case Direction::Right:
            return Position{static_cast<unsigned short>((point.x + amount) % width), point.y};
        case Direction::Left:
            return Position{static_cast<unsigned short>((point.x - amount) % width), point.y};
        default: 
            return point;
    }
}



bool Board::isSolidAt (Position point) const {
    return 'X' == (*matrix)[point.y][point.x].character;
}

