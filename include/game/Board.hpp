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
#include <random>

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

        void toggle_screen_active();

        vectorf32 get_sensorial_data (Position p);
        Position trueCoords (const Position p) const; 

        bool compare (const Position point, const char c) const;
        void set_cursor (const Position p, const char type, const bool put);
        Cell get_cell(const Position p) const;
        Position rand_empty_position (unsigned long seed);
        void rot_all ();

    private:
        float delta_time;
        bool scr_active;

        Position cursor_position;
        Cell cursor_is_over;
        char cursor_type;

        Position dimentions;
        unsigned short width;
        unsigned short height;

        std::unique_ptr<std::vector<std::vector<Cell>>> matrix;
        std::unique_ptr<std::vector<std::vector<float>>> auxiliar;

        //void swap() {std::swap(matrix, auxiliar);};
        void reset ();
        void display_delta_time () const;
};


