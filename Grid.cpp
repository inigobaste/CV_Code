#include "Grid.h"
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <utility>

// random grid constructor
// default is parallel
Grid::Grid(const int &num_rows, const int &num_cols, bool is_parallel) : nrows(num_rows), ncols(num_cols), parallel(is_parallel)
{
    int size = num_rows * num_cols;
    // reserve size so vector doesn't have to auto-resize
    this->cells.reserve(size);
    this->new_cells.reserve(size);

    if (is_parallel)
    {
#pragma omp parallel for
        for (int i = 0; i < size; i++)
            this->cells[i] = (rand() % 2);
    }
    else
    {
        for (int i = 0; i < size; i++)
            this->cells[i] = (rand() % 2);
    }
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

// return value indicates whether the game has reached steady state
bool Grid::do_iteration_parallel()
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

            int num_n = this->num_neighbours(i, j);
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

    // return true if the iteration has reached steady state
    if (this->cells.size() != this->new_cells.size())
    {
        return false;
    }
    // #pragma omp parallel for
    for (int i = 0; i < this->nrows * this->ncols; i++)
    {
        if (this->cells[i] != this->new_cells[i])
        {
            return false;
        }
    }
    return true;
}

bool Grid::do_iteration_serial()
{
    for (int i = 0; i < this->cells.size(); i++)
    {
        this->new_cells[i] = this->cells[i];
    }

    for (int i = 0; i < this->nrows; i++)
    {

        for (int j = 0; j < this->ncols; j++)
        {

            int num_n = this->num_neighbours(i, j);
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

    // return true if the iteration has reached steady state
    if (this->cells.size() != this->new_cells.size())
    {
        return false;
    }

    for (int i = 0; i < this->nrows * this->ncols; i++)
    {
        if (this->cells[i] != this->new_cells[i])
        {
            return false;
        }
    }
    return true;
}

bool Grid::do_iteration()
{
    if (this->parallel)
    {
        return this->do_iteration_parallel();
    }
    else
    {
        return this->do_iteration_serial();
    };
}

void Grid::time_data_to_file(const int &steps, const int &size, const double &time)
{
    std::string fname = this->parallel ? "parallel_time_data.dat" : "serial_time_data.dat";
    std::ofstream f1;
    f1.open(fname, std::ofstream::app);

    if (f1.is_open())
    {
        f1 << size << "\t" << steps << "\t" << time;
        f1 << std::endl;
    }

    f1.close();
}

// convert to sparse storage system
std::shared_ptr<COOGrid> Grid::dense_to_COO()
{
    std::vector<std::pair<int, int>> coords{};

    for (int i = 0; i < this->nrows; i++)
    {
        for (int j = 0; j < this->ncols; j++)
        {
            if (this->cells[i * this->ncols + j]) // check for nnz entries
            {
                std::cout << i << j << std::endl;
                coords.push_back(std::make_pair(i, j));
            }
        }
    }
    return std::make_shared<COOGrid>(this->nrows, this->ncols, coords);
}
