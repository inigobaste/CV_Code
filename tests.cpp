#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "Grid.h"
#include "COOGrid.h"
#include <vector>

// Test 1 : First Iteration for "Block" Creation: 5x6 grid forming a "block" after first iteration
// Tests elementwise accuracy for Grid::do_iteration(), COOGrid::do_iteration_serial(),  COOGrid::do_iteration_parallel()
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

    // do one iteration of Grid::do_iteration()
    Grid grid = Grid(5, 6, input);
    grid.do_iteration();

    // do one iteration of COOGrid::do_iteration_serial()
    Grid grid_COO = Grid(5, 6, input);
    std::shared_ptr<COOGrid> sparse_grid = grid_COO.dense_to_COO();
    sparse_grid->do_iteration_serial();
    std::shared_ptr<Grid> output_grid = sparse_grid->COO_to_dense();

    // do one iteration of COOGrid::do_iteration_parallel()
    Grid grid_COO1 = Grid(5, 6, input);
    std::shared_ptr<COOGrid> sparse_grid1 = grid_COO1.dense_to_COO();
    sparse_grid1->do_iteration_parallel();
    std::shared_ptr<Grid> output_grid1 = sparse_grid1->COO_to_dense();

    for (int i = 0; i < 30; i++)
    {
        // Test for Grid::do_iteration()
        CHECK(grid.cells[i] == expected[i]);
        // Test for COOGrid::do_iteration_serial()
        CHECK(output_grid->cells[i] == expected[i]);
        // Test for COOGrid::do_iteration_parallel()
        CHECK(output_grid1->cells[i] == expected[i]);
    }
}

// Test 2 : Test for 10x10 Grid: Elementwise accuracy check for an iteration over a 10x10 grid
// Tests elementwise accuracy for Grid::do_iteration(), COOGrid::do_iteration_serial(), COOGrid::do_iteration_parallel()
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

    // do one iteration of Grid::do_iteration()
    Grid grid = Grid(size, size, input);
    grid.do_iteration();

    // do one iteration of COOGrid::do_iteration_serial()
    Grid grid_COO = Grid(size, size, input);
    std::shared_ptr<COOGrid> sparse_grid = grid_COO.dense_to_COO();
    sparse_grid->do_iteration_serial();
    std::shared_ptr<Grid> output_grid = sparse_grid->COO_to_dense();

    // do one iteration of COOGrid::do_iteration_parallel()
    Grid grid_COO1 = Grid(size, size, input);
    std::shared_ptr<COOGrid> sparse_grid1 = grid_COO1.dense_to_COO();
    sparse_grid1->do_iteration_parallel();
    std::shared_ptr<Grid> output_grid1 = sparse_grid1->COO_to_dense();

    // Test for Grid::do_iteration()
    for (int i = 0; i < size * size; i++)
    {
        if (grid.cells[i] != expected[i])
        {
            std::cout << i << std::endl;
        }
        CHECK(grid.cells[i] == expected[i]);
    }

    // Test for COOGrid::do_iteration_serial()
    for (int i = 0; i < size * size; i++)
    {
        
        if (output_grid->cells[i] != expected[i])
        {
            std::cout << i << std::endl;
        }
        CHECK(output_grid->cells[i] == expected[i]);
    }

    // Test for COOGrid::do_iteration_parallel()
    for (int i = 0; i < size * size; i++)
    {
        
        if (output_grid1->cells[i] != expected[i])
        {
            std::cout << i << std::endl;
        }
        CHECK(output_grid1->cells[i] == expected[i]);
    }
}

// Test 3 : Steady State Check: Iterates twice over the first test's input to check 
    //if steady state is achieved (since the "block" does not change)
// Tests steady-state condition for Grid::do_iteration(), COOGrid::do_iteration_serial(), COOGrid::do_iteration_parallel()
TEST_CASE("returns true when it hits steady state")
{
    std::vector<bool> input = {0, 0, 0, 0, 0, 0,
                               0, 0, 1, 1, 0, 0,
                               0, 0, 0, 1, 0, 0,
                               0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0};

    // SS check for Grid::do_iteration()
    Grid grid = Grid(5, 6, input);
    bool first_iteration = grid.do_iteration();
    bool second_iteration = grid.do_iteration();
    CHECK(first_iteration == false);
    CHECK(second_iteration == true);

    // SS check for COOGrid::do_iteration_serial()
    Grid grid_COO = Grid(5, 6, input);
    std::shared_ptr<COOGrid> sparse_grid = grid_COO.dense_to_COO();
    bool first_it = sparse_grid->do_iteration_serial();
    bool second_it = sparse_grid->do_iteration_serial();
    CHECK(first_it == false);
    CHECK(second_it == true);

    // SS check for COOGrid::do_iteration_serial()
    Grid grid_COO1 = Grid(5, 6, input);
    std::shared_ptr<COOGrid> sparse_grid1 = grid_COO1.dense_to_COO();
    bool first_it1 = sparse_grid1->do_iteration_parallel();
    bool second_it1 = sparse_grid1->do_iteration_parallel();
    CHECK(first_it1 == false);
    CHECK(second_it1 == true);
}

// Test 3 : Generation Counter Check: Tests accuracy of generation counter 
// Tests Grid::do_iteration() only!
TEST_CASE("generation counter increments correctly")
{
    std::vector<bool> input = {0, 1, 0, 1};

    Grid grid = Grid(2, 2, input);
    grid.do_iteration();
    grid.do_iteration();

    CHECK(grid.generation == 2);
}
