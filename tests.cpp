#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "Grid.h"
#include <vector>

TEST_CASE("test the first iteration of the game of life")
{
    std::vector<bool> input = {0, 0, 0, 0, 0, 0,
                               0, 0, 1, 1, 0, 0,
                               0, 0, 0, 1, 0, 0,
                               0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0};
    std::vector<bool> expected = {0, 0, 0, 0, 0, 0,
                                  0, 0, 1, 1, 0, 0,
                                  0, 0, 1, 1, 0, 0,
                                  0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0};

    // do one iteration
    Grid grid = Grid(5, 6, input);
    grid.do_iteration();

    for (int i = 0; i < 30; i++)
    {
        CHECK(grid.cells[i] == expected[i]);
    }
}

TEST_CASE("test iteration for 10x10 grid")
{
    int size = 10;
    std::vector<bool> input = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
                               0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
                               1, 0, 0, 1, 1, 0, 0, 1, 0, 0,
                               0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                               0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
                               0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 1, 1, 0, 0, 0, 0};
    std::vector<bool> expected = {1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
                                  0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                  0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // do one iteration
    Grid grid = Grid(size, size, input);
    grid.do_iteration();

    for (int i = 0; i < size * size; i++)
    {
        if (grid.cells[i] != expected[i])
        {
            std::cout << i << std::endl;
        }
        CHECK(grid.cells[i] == expected[i]);
    }
}

TEST_CASE("returns true when it hits steady state")
{
    std::vector<bool> input = {0, 0, 0, 0, 0, 0,
                               0, 0, 1, 1, 0, 0,
                               0, 0, 0, 1, 0, 0,
                               0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0};

    Grid grid = Grid(5, 6, input);
    bool first_iteration = grid.do_iteration();
    bool second_iteration = grid.do_iteration();

    CHECK(first_iteration == false);
    CHECK(second_iteration == true);
}

TEST_CASE("generation counter increments correctly")
{
    std::vector<bool> input = {0, 1, 0, 1};

    Grid grid = Grid(2, 2, input);
    grid.do_iteration();
    grid.do_iteration();

    CHECK(grid.generation == 2);
}
