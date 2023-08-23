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
    width = maxX - 2;
    height = maxY - 2;

    scr_active = true;

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
        *matrix[point.y][point.x] = cell;
        *auxiliar[point.y][point.x] = cell;
    #else
        if (point.x >= width || point.y >= height) {
            endwin();
            std::ostringstream errorStream;
            errorStream << "Point (" << point.x << ", " << point.y << ") is outside of board (" << width << ", " << height << ")";
            throw std::runtime_error(errorStream.str());
        }
        (*matrix)[point.y % height][point.x % width] = cell;
        (*auxiliar)[point.y % height][point.x % width] = cell;
    #endif
}


static inline void drawOutline(int width, int height) {
    // Draw top border
    mvhline(0, 0, ACS_HLINE, width);

    // Draw bottom border
    mvhline(height - 1, 0, ACS_HLINE, width);

    // Draw left border
    mvvline(0, 0, ACS_VLINE, height);

    // Draw right border
    mvvline(0, width - 1, ACS_VLINE, height);

    // Draw corners
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, width - 1, ACS_URCORNER);
    mvaddch(height - 1, 0, ACS_LLCORNER);
    mvaddch(height - 1, width - 1, ACS_LRCORNER);
}

constexpr double SECONDS_TO_MILISECONDS = 1000.0;

void Board::display_delta_time () const {
    int value = static_cast<int>(SECONDS_TO_MILISECONDS * delta_time);

    // Display the value at the bottom corner
    mvprintw(height + 1, width - 22, " Delta-time: %d ", value);
}

void Board::render_static () const {
    for (size_t y = 0; y < matrix->size(); ++y) {
        for (size_t x = 0; x < (*matrix)[y].size(); ++x) {
            const Cell& cell = (*matrix)[y][x];
            mvaddch(1 + y, 1 + x, cell.character | COLOR_PAIR(cell.colorPair));
        }
    }
}

void Board::render () {
    drawOutline(width + 2, height + 2);

    if (scr_active) {render_static ();}

    display_delta_time ();

    refresh();

    //swap();
    //reset();
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

Position Board::trueCoords (const Position p) const {
    return Position {
        p.x % width,
        p.y % height
    };
}

// Sensorial extraction
std::vector<std::vector<float>> Board::getSensorialData (
    unsigned short x, 
    unsigned short y
) {
    std::vector<std::vector<float>> slice (5, std::vector<float>(5));

    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            Position p = trueCoords( Position{x + i, y + j} );

            slice[i + 2][j + 2] = cell_map( (*matrix)[p.y][p.x] );
        }
    }

    return slice;
}