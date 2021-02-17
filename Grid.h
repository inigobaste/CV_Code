#pragma once
#include <vector>
#include <memory>
#include <string>
class COOGrid;
#include "COOGrid.h"

// A grid in Conway's Game of Life
class Grid
{
private:
    int nrows = -1;
    int ncols = -1;
    bool parallel = true;
    int cores = 1;

    std::string new_data = "";

    bool do_iteration_parallel(bool iterate_string);
    bool do_iteration_serial(bool iterate_string);

public:
    std::string data = "";
    std::vector<bool> cells;
    std::vector<bool> new_cells;

    int generation = 0;

    Grid(const int &num_rows, const int &num_cols, bool is_parallel, int &n_cores);
    Grid(const int &num_rows, const int &num_cols, std::vector<bool> &cell_states, bool is_parallel = true);

    ~Grid();

    int num_neighbours(const int &ii, const int &jj);
    void to_file(const int &it);
    bool do_iteration(bool iterate_string = true);
    void time_data_to_file(const int &steps, const int &size, const double &time);
    std::shared_ptr<COOGrid> dense_to_COO();
};
