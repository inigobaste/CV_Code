#include "COOGrid.h"
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <memory>

// constructor with x and y values passed in
// TODO: change this so it doesn't copy the vectors
COOGrid::COOGrid(const int &num_rows, const int &num_cols, std::vector<std::pair<int, int>> ij, bool is_parallel) : nrows(num_rows), ncols(num_cols), coords(ij), parallel(is_parallel)
{
}

COOGrid::~COOGrid()
{
}

bool COOGrid::do_iteration_serial()
{
    std::map<std::pair<int, int>, int> indexMap; // counts repetitions of coords as value
    int nnzs = this->coords.size();

    for (int k = 0; k < nnzs; k++)
    {
        int x;
        int y;

        // destructure x and y
        std::tie(x, y) = this->coords[k];

        // give live cells +10 occurrences
        if (indexMap[this->coords[k]])
        {
            indexMap[this->coords[k]] += 100;
        }
        else
        {
            indexMap[this->coords[k]] = 100;
        }

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }

                // for all neighbors add +1 occurrence
                std::pair<int, int> ij = std::make_pair((x + i + this->nrows) % this->nrows, (y + j + this->ncols) % this->ncols);

                if (indexMap[ij])
                {
                    indexMap[ij] += 1;
                }
                else
                {
                    indexMap[ij] = 1;
                }
            }
        }
    }

    // A SPARSITY CALCULATOR
    float sparsity;
    sparsity = nnzs / (float)(this->nrows * this->ncols);
    std::cout << "Sparsity = " << sparsity << "\n";

    // any coordinates that fulfill the criteria are added to a new array;
    for (std::map<std::pair<int, int>, int>::iterator it = indexMap.begin(); it != indexMap.end(); it++)
    {
        int occurrences = it->second;

        if (occurrences == 3 || occurrences == 102 || occurrences == 103)
        {
            std::pair<int, int> pair = it->first;

            this->new_coords.push_back(pair);
        }
    }

    this->coords.swap(this->new_coords);

    // return true if the iteration has reached steady state
    if (coords.size() != new_coords.size())
    {
        new_coords.clear();
        return false;
    }
    // #pragma omp parallel for
    for (int i = 0; i < coords.size(); i++)
    {
        if ((coords[i].first != new_coords[i].first) || (coords[i].second != new_coords[i].second))
        {
            new_coords.clear();
            return false;
        }
    }
    new_coords.clear();
    return true;
}

// convert to dense storage system
std::shared_ptr<Grid> COOGrid::COO_to_dense()
{
    std::vector<bool> cells((this->nrows * this->ncols), false); // clear the vector
    for (int i = 0; i < this->coords.size(); i++)
    {
        cells[this->coords[i].first * this->ncols + this->coords[i].second] = true;
    }

    return std::make_shared<Grid>(this->nrows, this->ncols, false);
}
