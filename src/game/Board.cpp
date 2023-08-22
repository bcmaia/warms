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

    atexit(Board::cleanupNcurses);
}

Board::~Board() {
    //endwin();
}

// void Board::printat (Position point, Cell cell) {
//     (*matrix)[point.y][point.x] = cell;
//     mvaddch(point.y, point.x, cell.character | COLOR_PAIR(cell.colorPair));
// }

#include <iostream>
#include <cstdlib>
#include <execinfo.h> // For stack trace functions

void printStackTrace() {
    void* callstack[128];
    int frames = backtrace(callstack, sizeof(callstack) / sizeof(callstack[0]));
    char** strs = backtrace_symbols(callstack, frames);

    std::cout << "Stack Trace:" << std::endl;
    for (int i = 0; i < frames; i++) {
        std::cout << strs[i] << std::endl;
    }

    free(strs);
}


    
void Board::setcell(Position point, Cell cell) {
    #ifdef RELEASE
        *auxiliar[point.y][point.x] = cell;
    #else
        if (point.x >= width || point.y >= height) {
            endwin();
            std::ostringstream errorStream;
            errorStream << "Point (" << point.x << ", " << point.y << ") is outside of board (" << width << ", " << height << ")";
            throw std::runtime_error(errorStream.str());
        }
        (*auxiliar)[point.y % height][point.x % width] = cell;
    #endif
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

Position Board::movement (Position point, Direction dir, unsigned short amount = 1) const {
    switch (dir) {
        case Direction::Down: {
            return Position{point.x, static_cast<unsigned short>((point.y + amount) % height)};
        }

        case Direction::Up: {
            // TODO: Simplify this mess
            int a = static_cast<int>(amount);
            int y = static_cast<int>(point.y);
            int h = static_cast<int>(height);
            int p = ((y - a) % h + h) % h; // constly, but always correct
            return Position{point.x, static_cast<unsigned short>(p)};
        }

        case Direction::Right: {
            return Position{static_cast<unsigned short>((point.x + amount) % width), point.y};
        }

        case Direction::Left: {
            // TODO: Simplify this mess
            int a = static_cast<int>(amount);
            int x = static_cast<int>(point.x);
            int w = static_cast<int>(width);
            int p = ((x - a) % w + w) % w; // constly, but always correct
            
            return Position{static_cast<unsigned short>(p), point.y};
        }

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