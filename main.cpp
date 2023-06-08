#include <iostream>
#include <ncurses.h>

#include "Grid.h"
#include <chrono>
#include <thread>
#include <unistd.h>
#include <chrono>
#include <string>
typedef struct cmd_options
{
    int rows;
    int cols;
    int frame_rate;
    int random_seed;
} cmd_options_t;

void fill_grid(Grid grid)
{
    auto& cg = *grid.get_current_grid();
    for (unsigned int row = 0; row < cg.size(); row++)
    {
        for (unsigned int col = 0; col < cg[row].size(); col++)
        {
            cg[row][col].set_type(static_cast<UnitType>(rand() % static_cast<int>(UnitType::NONE)));
        }
    }
}

void grid_test(Grid grid)
{
    auto& cg = *grid.get_current_grid();
    cg[cg.size() / 2][0].set_type(UnitType::ROCK);
    cg[cg.size() / 2][cg[0].size() - 1].set_type(UnitType::SCISSORS);
}

cmd_options_t parse_args(int argc, char* argv[])
{
    int c;
    cmd_options_t cmd_options;
    cmd_options.rows = -1;
    cmd_options.cols = -1;
    cmd_options.frame_rate = 1000;
    cmd_options.random_seed = time(NULL);
    while((c = getopt(argc, argv, "r:c:f:s:")) != -1)
    {
        switch(c)
        {
            case 'r':
                cmd_options.rows = atoi(optarg);
                break;
            case 'c':
                cmd_options.cols = atoi(optarg);
                break;
            case 'f':
                cmd_options.frame_rate = atoi(optarg);
                break;
            case 's':
                cmd_options.random_seed = atoi(optarg);
        }
    }
    return cmd_options;
}


#define NORMAL_PAIR 1
#define HIGHLIGHT_PAIR 2
#define ROCK_PAIR 3
#define PAPER_PAIR 4
#define SCISSORS_PAIR 5

void draw_grid(Grid* grid, WINDOW* window)
{
    auto& current_grid = *grid->get_current_grid();
    for (unsigned int row = 0 ; row < current_grid.size(); row++)
    {
        for (unsigned int col = 0; col < current_grid[row].size(); col++)
        {
            UnitType type = grid->get_unit_type(row, col);
            int pair;
            switch(type)
            {
                case UnitType::ROCK:
                    pair = COLOR_PAIR(ROCK_PAIR);
                    break;
                case UnitType::PAPER:
                    pair = COLOR_PAIR(PAPER_PAIR);
                    break;
                case UnitType::SCISSORS:
                    pair = COLOR_PAIR(SCISSORS_PAIR);
                    break;
                case UnitType::NONE:
                    pair = COLOR_PAIR(NORMAL_PAIR);
                    break;
                case UnitType::KABOOM:
                    pair = COLOR_PAIR(HIGHLIGHT_PAIR);
                    break;
            }
            wattron(window, pair);
            mvwaddch(window, row, col, grid->get_char(row, col));
            wattroff(window, pair);
        }
    }
}

void init_ncurses()
{
    initscr();
    noecho();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);

}

int main(int argc, char *argv[]) {
    WINDOW* inner_window = nullptr;
    WINDOW* outer_window = nullptr;
    WINDOW* status_bar = nullptr;
    init_ncurses();
    auto frame_start = std::chrono::high_resolution_clock::now();

    cmd_options_t cmd_options = parse_args(argc, argv);
    timeout(cmd_options.frame_rate);

    srand(cmd_options.random_seed);
    if (cmd_options.rows == -1 || cmd_options.cols == -1)
    {
        getmaxyx(stdscr, cmd_options.rows, cmd_options.cols);
    }
    inner_window = newwin(cmd_options.rows - 3, cmd_options.cols - 1, 1, 1);
    outer_window = newwin(cmd_options.rows, cmd_options.cols, 0, 0);
    status_bar = newwin(1, cmd_options.cols - 2, cmd_options.rows - 2, 1);
    wtimeout(inner_window, 0);

    auto grid = Grid(cmd_options.rows - 3, cmd_options.cols - 2);
    //fill_grid(grid);
    grid_test(grid);

    char val = '\0';
    long duration = 0;
    while(val != 'q')
    {
        frame_start = std::chrono::high_resolution_clock::now();
        box(outer_window, 0, 0);
        wrefresh(outer_window);
        wattron(status_bar, COLOR_PAIR(2));
        mvwprintw(status_bar, 0, 0, "Duration: %ld", duration);
        wattroff(status_bar, COLOR_PAIR(2));
        draw_grid(&grid, inner_window);

        wrefresh(inner_window);
        wrefresh(status_bar);
        val = wgetch(inner_window);
        grid.process_turn();
        auto frame_end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds >(frame_end - frame_start).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(cmd_options.frame_rate - duration));

    }

    endwin();
    return 0;
}
