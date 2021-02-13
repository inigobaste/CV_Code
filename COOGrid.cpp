#include "COOGrid.h"
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

// constructor with x and y values passed in
// TODO: change this so it doesn't copy the vectors
COOGrid::COOGrid(const int &num_rows, const int &num_cols, std::vector<int> x, std::vector<int> y, bool is_parallel) : nrows(num_rows), ncols(num_cols), x_coo(x), y_coo(y), parallel(is_parallel)
{
}

COOGrid::~COOGrid()
{
}
