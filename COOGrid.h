#pragma once
#include <vector>
#include <memory>
#include <utility>

// declare Grid class to avoid circular import error
class Grid;
#include "Grid.h"

// A sparse grid in Conway's Game of Life
class COOGrid
{
private:
    int nrows = -1;
    int ncols = -1;
    bool parallel = true;

    // vector of pairs of (x, y) coordinates of live cells
    std::vector<std::pair<int, int>> coords;
    // temporary storage of updated coordinates
    std::vector<std::pair<int, int>> new_coords{};

public:
    COOGrid(const int &num_rows, const int &num_cols, std::vector<std::pair<int, int>> ij, bool is_parallel = true);
    ~COOGrid();
    // compute next generation of the grid cell states in serial
    bool do_iteration_serial();
    // compute next generation of the grid cell states in parallel
    bool do_iteration_parallel();
    // shared pointer, converts COOGrid object to Grid object
    std::shared_ptr<Grid> COO_to_dense();
};
