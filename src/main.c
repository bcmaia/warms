// #include <ncurses.h>
// #include <cstdlib>
// #include <chrono>
// #include <thread>

// int main() {
//     setenv("LANG", "en_US.UTF-8", 1);

//     initscr();
//     cbreak();
//     noecho();
//     keypad(stdscr, TRUE);
//     curs_set(FALSE);
//     nodelay(stdscr, TRUE);

//     start_color();
//     init_pair(1, COLOR_GREEN, COLOR_BLACK);

//     int x = 0, y = 0;
//     int maxX, maxY;

//     getmaxyx(stdscr, maxY, maxX);

//     // Calculate desired frame duration for 100 fps
//     constexpr double targetFrameDuration = 1.0 / 100.0; // 100 fps
//     using Clock = std::chrono::high_resolution_clock;
//     std::chrono::duration<double> frameDuration(targetFrameDuration);

//     while (true) {
//         auto frameStart = Clock::now();

//         clear();
//         mvprintw(y, x, "*");
//         refresh();

//         int ch = getch();
        
//         if (ch == 'q') {
//             break;
//         } else if (ch == KEY_UP && y > 0) {
//             y--;
//         } else if (ch == KEY_DOWN && y < maxY - 1) {
//             y++;
//         } else if (ch == KEY_LEFT && x > 0) {
//             x--;
//         } else if (ch == KEY_RIGHT && x < maxX - 1) {
//             x++;
//         }
        
//         auto frameEnd = Clock::now();
//         std::chrono::duration<double> frameTime = frameEnd - frameStart;

//         // Calculate sleep duration to maintain desired frame rate
//         std::chrono::duration<double> sleepDuration = frameDuration - frameTime;
//         if (sleepDuration > std::chrono::duration<double>(0)) {
//             std::this_thread::sleep_for(sleepDuration);
//         }
//     }

//     endwin();

//     return 0;
// }










#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <thread>

#include "types.hpp"


void printAtPosition(int x, int y, char character, int colorPair, std::vector<std::vector<Cell>>& buffer) {
    buffer[y][x] = {character, colorPair};
    //mvaddch(y, x, character | COLOR_PAIR(colorPair));
}

void printAtPositionAndRender(int x, int y, char character, ColorPair colorPair, std::vector<std::vector<Cell>>& buffer) {
    buffer[y][x] = {character, colorPair};
    mvaddch(y, x, character | COLOR_PAIR(colorPair));
}

void renderBuffer(const std::vector<std::vector<Cell>>& buffer) {
    for (size_t y = 0; y < buffer.size(); ++y) {
        for (size_t x = 0; x < buffer[y].size(); ++x) {
            const Cell& cell = buffer[y][x];
            mvaddch(y, x, cell.character | COLOR_PAIR(cell.colorPair));
        }
    }
    refresh();
}

int main() {
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

    // Initialize buffer with empty cells
    std::vector<std::vector<Cell>> buffer(maxY, std::vector<Cell>(maxX, {' ', 0}));

    int x = 0, y = 0;

    // Calculate desired frame duration for 100 fps
    constexpr double targetFrameDuration = 1.0 / 100.0; // 100 fps
    using Clock = std::chrono::high_resolution_clock;
    std::chrono::duration<double> frameDuration(targetFrameDuration);

    while (true) {
        auto frameStart = Clock::now();
        //clear();


        printAtPositionAndRender(x, y, '*', 1, buffer);

        int ch = getch();

        if (ch == 'q') {
            break;
        } else if (ch == KEY_UP && y > 0) {
            y--;
        } else if (ch == KEY_DOWN && y < maxY - 1) {
            y++;
        } else if (ch == KEY_LEFT && x > 0) {
            x--;
        } else if (ch == KEY_RIGHT && x < maxX - 1) {
            x++;
        }

        //renderBuffer(buffer);

        auto frameEnd = Clock::now();
        std::chrono::duration<double> frameTime = frameEnd - frameStart;

        // Calculate sleep duration to maintain desired frame rate
        std::chrono::duration<double> sleepDuration = frameDuration - frameTime;
        if (sleepDuration > std::chrono::duration<double>(0)) {
            std::this_thread::sleep_for(sleepDuration);
        }
    }

    endwin();

    return 0;
}
