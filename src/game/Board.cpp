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

    m1.resize(height, std::vector<Cell>(width, Cell{' ', 0}));
    m2.resize(height, std::vector<Cell>(width, Cell{' ', 0}));

    matrix = &m1;
    auxiliar = &m2;
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


// bool Board::isSolidAt (Position point) const {
//     std::vector<std::vector<cell>> subarray(5, std::vector<cell>(5));

//     int matrixWidth = matrix[0].size();
//     int matrixHeight = matrix.size();

//     for (int i = -2; i <= 2; ++i) {
//         for (int j = -2; j <= 2; ++j) {
//             int rowIndex = (x + i + matrixHeight) % matrixHeight;
//             int colIndex = (y + j + matrixWidth) % matrixWidth;
//             subarray[i + 2][j + 2] = matrix[rowIndex][colIndex];
//         }
//     }

//     return subarray;
// }

void Board::displayValue(int value) {
    // Display the value at the bottom corner
    mvprintw(height - 1, width - 10, "Value: %d", value);
    refresh();
}