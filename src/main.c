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
#include "Game.hpp"


int main() {
    unsigned long seed = 1;
    unsigned population_size = 10;

    Game game(seed, population_size);
    game.run();

    return 0;
}
