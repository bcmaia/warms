#include "Board.hpp"

void initColorPairs() {
    if (has_colors() && COLOR_PAIRS >= 16) {
        // Initialize color pairs with black background
        for (int i = 0; i < 16; ++i) {
            init_pair(i + 1, i, COLOR_BLACK);
        }
    } else {
        throw std::runtime_error("Not enough color support!\n");
    }

    refresh();
}


Board::Board() {
    setenv("LANG", "en_US.UTF-8", 1);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    nodelay(stdscr, TRUE);

    start_color();
    initColorPairs();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);
    width = maxX - 2;
    height = maxY - 2;
    dimentions = Position{width, height};

    scr_active = true;

    matrix = std::make_unique<std::vector<std::vector<Cell>>>(
        height, std::vector<Cell>(width, Cell{' ', 1})
    );

    float empty_cell_float_value = Cell{' ', 1}.to_float();
    auxiliar = std::make_unique<std::vector<std::vector<float>>>(
        height, std::vector<float>(width, empty_cell_float_value)
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
        (*auxiliar)[point.y][point.x] = cell.to_float();
        (*matrix)[point.y][point.x] = cell;

    #else
        if (point.x >= width || point.y >= height) {
            endwin();
            std::ostringstream errorStream;
            errorStream << "Point (" << point.x << ", " << point.y << ") is outside of board (" << width << ", " << height << ")";
            throw std::runtime_error(errorStream.str());
        }
        (*auxiliar)[point.y % height][point.x % width] = cell.to_float();
        (*matrix)[point.y % height][point.x % width] = cell;
    #endif

    if (scr_active) mvaddch(1 + point.y, 1 + point.x, cell.character | COLOR_PAIR(cell.colorPair));
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

void Board::rot_all () {
    for (size_t y = 0; y < matrix->size(); ++y) {
        for (size_t x = 0; x < (*matrix)[y].size(); ++x) {
            Cell& cell = (*matrix)[y][x];
            if ('6' == cell.character || '&' == cell.character) cell = Cell{' ', 0};
            mvaddch(1 + y, 1 + x, cell.character | COLOR_PAIR(cell.colorPair));
        }
    }
}

void Board::render () {
    if (scr_active) drawOutline(width + 2, height + 2);

    //if (scr_active) {render_static ();}

    // Render cursor
    mvaddch(1 + cursor_position.y, 1 + cursor_position.x, cursor_type | COLOR_PAIR(1));

    display_delta_time ();

    refresh();

    //swap();
    //reset();
}

void Board::toggle_screen_active() {
    scr_active = !scr_active;

    drawOutline(width + 2, height + 2);
    render_static ();
    mvaddch(1 + cursor_position.y, 1 + cursor_position.x, cursor_type | COLOR_PAIR(1));
    display_delta_time ();
    refresh();
}

void Board::reset () {
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            (*matrix)[i][j] = Cell{' ', 0};
            (*auxiliar)[i][j] = Cell{' ', 0}.to_float();
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
    return (
        ('*' == (*matrix)[point.y][point.x].character)
        || ('@' == (*matrix)[point.y][point.x].character)
        || ('+' == (*matrix)[point.y][point.x].character)
        || ('#' == (*matrix)[point.y][point.x].character)
    );
}

bool Board::compare (const Position point, const char c) const {
    return c == (*matrix)[point.y][point.x].character;
}


// Sensorial extraction
vectorf32 Board::get_sensorial_data (Position p) {
    vectorf32 slice;
    slice.reserve(16);

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            Position point = (p + Position{j, i}).mold(dimentions);
            slice.push_back( (*auxiliar)[point.y][point.x] );
        }
    }

    return slice;
}

Cell Board::get_cell(const Position p) const {
    #ifdef RELEASE
        return (*matrix)[p.y][p.x];
    #else
        if (p.x >= width || p.y >= height) {
            endwin();
            std::ostringstream errorStream;
            errorStream << "Point (" << p.x << ", " << p.y << ") is outside of board (" << width << ", " << height << ")";
            throw std::runtime_error(errorStream.str());
        }
        return (*matrix)[p.y % height][p.x % width];
    #endif
}

void Board::set_cursor (const Position p, const char type, const bool put) {
    if (cursor_position != p) {
        if (scr_active) mvaddch(1 + cursor_position.y, 1 + cursor_position.x, cursor_is_over.character | COLOR_PAIR(cursor_is_over.colorPair));
        else mvaddch(1 + cursor_position.y, 1 + cursor_position.x, ' ' | COLOR_PAIR(0));
        cursor_is_over = get_cell(p);
    };

    cursor_position = p;
    cursor_type = type;

    if (!put) return;

    char current = get_cell(p).character;

    if ('#' == type) {
        cursor_is_over = '#' == current ? Cell{' ', 0} : Cell{'#', 1};
        setcell(p, cursor_is_over);
    } else if ('X' == type) {
        cursor_is_over = Cell('6', 1);
        setcell(p, cursor_is_over);
    }
}


Position Board::rand_empty_position (unsigned long seed) {
    int w = (int)width; // Minimum value
    int h = (int)height; 

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis_width (0, w - 1);
    std::uniform_int_distribution<int> dis_height (0, h - 1);

    int search_count = 100;
    while (0 < search_count--) {
        int x = dis_width(gen);
        int y = dis_height(gen);
        Position p = Position{x, y};

        if (!isSolidAt(p)) return p;
    }

    return Position{0, 0};
}