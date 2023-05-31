#include <iostream>
#include <ncurses.h>

#include "Grid.h"
#include <chrono>
#include <thread>
#include <unistd.h>

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
    for (int row = 0; row < cg.size(); row++)
    {
        for (int col = 0; col < cg[row].size(); col++)
        {
            cg[row][col] = static_cast<UnitType>(rand() % static_cast<int>(UnitType::NONE));
        }
    }
}

cmd_options_t parse_args(int argc, char* argv[])
{
    int c;
    cmd_options_t cmd_options;
    cmd_options.rows = -1;
    cmd_options.cols = -1;
    cmd_options.frame_rate = 250;
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


void draw_grid(Grid* grid, WINDOW* window)
{
    auto& current_grid = *grid->get_current_grid();
    for (int row = 0 ; row < current_grid.size(); row++)
    {
        for (int col = 0; col < current_grid[row].size(); col++)
        {
            mvwaddch(window, row, col, grid->get_string(row, col));
        }
    }
}

void init_ncurses()
{
    initscr();
    noecho();
    cbreak();
}

int main(int argc, char *argv[]) {
    WINDOW* window = nullptr;
    init_ncurses();

    cmd_options_t cmd_options = parse_args(argc, argv);
    timeout(cmd_options.frame_rate);
    srand(cmd_options.random_seed);
    if (cmd_options.rows == -1 || cmd_options.cols == -1)
    {
        getmaxyx(stdscr, cmd_options.rows, cmd_options.cols);
    }
    window = newwin(cmd_options.rows, cmd_options.cols, 0, 0);
    wrefresh(window);
    //mvwprintw(stdscr, 0, 0, "Max X: %d", cmd_options.rows);
    //mvwprintw(stdscr, 1, 0, "Max Y: %d", cmd_options.cols);
    refresh();
    getch();

    auto grid = Grid(cmd_options.rows, cmd_options.cols);
    fill_grid(grid);
    /*
    grid.add_unit(5, 5, UnitType::ROCK);
    grid.add_unit(5, 6, UnitType::PAPER);
    grid.add_unit(5, 7, UnitType::SCISSORS);
    grid.add_unit(5, 8, UnitType::ROCK);
    grid.add_unit(5, 9, UnitType::PAPER);
    grid.add_unit(5, 10, UnitType::SCISSORS);
    */
    char val = '\0';
    while(val != 'q')
    {
        draw_grid(&grid, window);
        wrefresh(window);
        val = getch(); // We use timeout (above) to both wait for input, and to create our framerate.
        grid.process_turn();
    }

    endwin();
    return 0;
}
