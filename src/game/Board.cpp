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

    matrix = std::make_unique<std::vector<std::vector<Cell>>>(height, std::vector<Cell>(width));
    auxiliar = std::make_unique<std::vector<std::vector<Cell>>>(height, std::vector<Cell>(width));
}

Board::~Board() {
    width = 0;
    height = 0;

    matrix = NULL;
    auxiliar = NULL;

    endwin();
}

void Board::printat (Position point, Cell cell) {
    (*matrix)[point.y][point.x] = cell;
    mvaddch(point.y, point.x, cell.character | COLOR_PAIR(cell.colorPair));
}

void Board::setcell (Position point, Cell cell) {
    (*matrix)[point.y][point.x] = cell;
}

void Board::render () const {
    for (size_t y = 0; y < matrix->size(); ++y) {
        for (size_t x = 0; x < (*matrix)[y].size(); ++x) {
            const Cell& cell = (*matrix)[y][x];
            mvaddch(y, x, cell.character | COLOR_PAIR(cell.colorPair));
        }
    }
    refresh();
}