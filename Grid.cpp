#include "Grid.h"
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

// random grid constructor
Grid::Grid(const int &num_rows, const int &num_cols) : nrows(num_rows), ncols(num_cols)
{
    int size = num_rows * num_cols;
    // reserve size so vector doesn't have to auto-resize
    this->cells.reserve(size);
    this->new_cells.reserve(size);

#pragma omp parallel for
    for (int i = 0; i < size; i++)
        this->cells[i] = (rand() % 2);
}

Grid::~Grid()
{
}

int Grid::num_neighbours(const int &ii, const int &jj)
{
    int ix, jx;
    int cnt = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (i != 0 || j != 0)
            {
                ix = (i + ii + this->nrows) % this->nrows;
                jx = (j + jj + this->ncols) % this->ncols;
                if (this->cells[ix * this->ncols + jx])
                    cnt++;
            }
    return cnt;
}

void Grid::to_file(const int &it)
{
    std::stringstream fname;
    std::fstream f1;

    fname << "output"
          << "_" << it << ".dat";

    f1.open(fname.str().c_str(), std::ios_base::out);

    for (int i = 0; i < this->nrows; i++)
    {
        for (int j = 0; j < this->ncols; j++)
            f1 << this->cells[i * this->ncols + j] << "\t";
        f1 << std::endl;
    }
    f1.close();
}

void Grid::do_iteration()
{

#pragma omp parallel for
    for (int i = 0; i < this->cells.size(); i++)
    {
        this->new_cells[i] = this->cells[i];
    }

#pragma omp parallel for

    for (int i = 0; i < this->nrows; i++)
    {

        for (int j = 0; j < this->ncols; j++)
        {

            int num_n = num_neighbours(i, j);
            if (this->cells[i * this->ncols + j])
            {
                if (num_n != 2 && num_n != 3)
                    this->new_cells[i * this->ncols + j] = false;
            }
            else if (num_n == 3)
                this->new_cells[i * this->ncols + j] = true;
        }
    }

    // threading ends here
    this->cells.swap(this->new_cells);
}

void Grid::time_data_to_file(const int &steps, const int &size, const double &time)
{
    std::string fname = "time_data.dat";
    std::ofstream f1;
    f1.open(fname, std::ofstream::app);

    if (f1.is_open())
    {
        f1 << size << "\t" << steps << "\t" << time;
        f1 << std::endl;
    }

    f1.close();
}
