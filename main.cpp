#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <omp.h>
#include <string>
#include "Grid.h"
#include "doctest.h"
#include "Grid.cpp"
#include "COOGrid.cpp"
#include "FileWriter.h"

void grid_to_file(int it, std::vector<bool> grid_input, int dim)
{
    std::stringstream fname;
    std::fstream f1;

    fname << "output"
          << "_" << it << ".dat";

    f1.open(fname.str().c_str(), std::ios_base::out);

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
            f1 << grid_input[i * dim + j] << "\t";
        f1 << std::endl;
    }
    f1.close();
}

double run_time, start_time;
int max_steps = 1200;

int main(int argc, char **argv)
{
    doctest::Context context;
    // defaults: https://github.com/onqtam/doctest/blob/master/doc/markdown/main.md
    context.setOption("abort-after", 5);   // stop test execution after 5 failed assertions
    context.setOption("order-by", "name"); // sort the test cases by their name

    // add --no-run flag to run program without tests
    context.applyCommandLine(argc, argv);
    context.run();

    std::ofstream ofs;
    ofs.open("time_data.dat", std::ofstream::trunc);

    std::ofstream ofs;
    ofs.open("parallel_time_data.dat", std::ofstream::trunc);

    ofs.close();

    std::vector<int> dims{1000};

    for (int dim : dims)
    {
        std::cout << "Dimension: " << dim << std::endl;
        // create a random grid
        Grid grid = Grid(dim, dim, true);

        start_time = omp_get_wtime();
        int threads = 12;

        std::vector<bool> store_grids;
        int ID;
        int cnt = 0;
        std::cout << grid.cells.size() << std::endl;

        for (int n = 0; n < max_steps; n++)
        {

            bool isSteadyState = grid.do_iteration();
            if (isSteadyState)
            {
                std::cout << "Steady state was reached." << std::endl;
                break;
            }

            store_grids.resize(grid.cells.size() * threads);
            //             // sparse_grid->do_iteration();
            // grid.to_file(n);

#pragma omp parallel for
            for (int i = cnt * grid.cells.size(); i < ((cnt + 1) * grid.cells.size()); i++)
            {
                store_grids[i] = grid.cells[i % grid.cells.size()];
            }

            if ((n + 1) % threads == 0)
            {
#pragma omp parallel for
                for (int i = 0; i < threads; i++)
                {
                    // std::cout << "ID = " << ID << "\n";
                    std::vector<bool>::const_iterator first = store_grids.begin() + i * grid.cells.size();
                    std::vector<bool>::const_iterator last = store_grids.begin() + (i + 1) * grid.cells.size();
                    std::vector<bool> v(first, last);

                    // std::cout << "v_size = " << v.size() << "\n";

                    grid_to_file(i + (n + 1) - threads, v, dim);
                }
                cnt = -1;
                store_grids.clear();
            }

            run_time = omp_get_wtime() - start_time;
            grid.time_data_to_file(n, dim, run_time);
            cnt++;
        }
    }

    return 0;
}