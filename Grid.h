#include <vector>
#include <memory>
#include "COOGrid.h"
#include <string>

// A grid in Conway's Game of Life
class Grid
{
private:
    int nrows = -1;
    int ncols = -1;
    bool parallel = true;

    std::string data = "";
    std::string new_data = "";

    std::vector<bool> cells;
    std::vector<bool> new_cells;

    bool do_iteration_parallel();
    bool do_iteration_serial();

public:
    Grid(const int &num_rows, const int &num_cols, bool is_parallel = true);
    ~Grid();
    int num_neighbours(const int &ii, const int &jj);
    void to_file(const int &it);
    bool do_iteration();
    void time_data_to_file(const int &steps, const int &size, const double &time);
    std::shared_ptr<COOGrid> dense_to_COO();
};
