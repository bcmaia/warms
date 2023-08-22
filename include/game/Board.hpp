#pragma once

#include <vector>

#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <stdexcept>
#include <sstream>

#include "types.hpp"

class Board {
    public:
        Board();
        ~Board();

        unsigned short getWidth () const {return width;};
        unsigned short getHeight () const {return height;};

        //void printat (Position point, Cell cell);
        void setcell (Position point, Cell cell);
        void render_static () const;

        void render ();

        bool isSolidAt (Position point) const;

        Position movement (Position point, Direction dir, unsigned short amount) const;

        void set_delta_time(float value) {delta_time = value;};

        static void cleanupNcurses() {
            endwin(); // Terminate ncurses mode and clean up resources
        }

        void toggle_screen_active() {
            scr_active = !scr_active;
        }

    private:
        float delta_time;
        bool scr_active;

        unsigned short width;
        unsigned short height;

        std::unique_ptr<std::vector<std::vector<Cell>>> matrix;
        std::unique_ptr<std::vector<std::vector<Cell>>> auxiliar;

        void swap() {std::swap(matrix, auxiliar);};
        void reset ();
        void display_delta_time () const;
};