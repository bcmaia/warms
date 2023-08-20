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
#include "Board.hpp"


int main() {
    //========================================================================//
    //=================|    INITIALIZATION    |===============================//
    //========================================================================//
    
    Board board;

    int x = 0, y = 0;

    // Calculate desired frame duration for 100 fps
    constexpr double targetFrameDuration = 1.0 / 100.0; // 100 fps
    using Clock = std::chrono::high_resolution_clock;
    std::chrono::duration<double> frameDuration(targetFrameDuration);


    //========================================================================//
    //=================|    GAME LOOP    |====================================//
    //========================================================================//
    while (true) {
        auto frameStart = Clock::now();
        //clear();


        board.printat(
            Position{static_cast<short unsigned int>(x), static_cast<short unsigned int>(y)},
            Cell{'*', 1}
        );

        int ch = getch();

        if (ch == 'q') {
            break;
        } else if (ch == KEY_UP && y > 0) {
            y--;
        } else if (ch == KEY_DOWN && y < board.getHeight() - 1) {
            y++;
        } else if (ch == KEY_LEFT && x > 0) {
            x--;
        } else if (ch == KEY_RIGHT && x < board.getWidth() - 1) {
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

    return 0;
}
