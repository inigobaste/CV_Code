#include <vector>

// A sparse grid in Conway's Game of Life
class COOGrid
{
private:
    int nrows = -1;
    int ncols = -1;
    bool parallel = true;

    std::vector<int> x_coo;
    std::vector<int> y_coo;

public:
    COOGrid(const int &num_rows, const int &num_cols, std::vector<int> x, std::vector<int> y, bool is_parallel = true);
    ~COOGrid();
};
