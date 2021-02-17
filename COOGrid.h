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

    std::vector<std::pair<int, int>> coords;
    std::vector<std::pair<int, int>> new_coords{};

public:
    COOGrid(const int &num_rows, const int &num_cols, std::vector<std::pair<int, int>> ij, bool is_parallel = true);
    ~COOGrid();
    bool do_iteration_serial();
    bool do_iteration_parallel();
    std::shared_ptr<Grid> COO_to_dense();
};
