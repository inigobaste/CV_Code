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

using namespace std;

double run_time, start_time;
int max_steps = 1;

int main()
{
    doctest::Context context;
    // defaults: https://github.com/onqtam/doctest/blob/master/doc/markdown/main.md
    context.setOption("abort-after", 5);   // stop test execution after 5 failed assertions
    context.setOption("order-by", "name"); // sort the test cases by their name
    context.run();

    ofstream ofs;
    ofs.open("time_data.dat", std::ofstream::trunc);
    ofs.close();

    vector<int> dims{10, 20, 100}; //, 500, 1000, 5000

    for (int dim : dims)
    {
        cout << "Dimension: " << dim << endl;
        // create a random grid
        Grid grid = Grid(dim, dim, true);

        start_time = omp_get_wtime();
        srand(time(NULL));

        for (int n = 0; n < max_steps; n++)
        {
            bool isSteadyState = grid.do_iteration();
            if (isSteadyState)
            {
                std::cout << "Steady state was reached." << std::endl;
                break;
            }

            grid.to_file(n);
        }

        run_time = omp_get_wtime() - start_time;
        grid.time_data_to_file(max_steps, dim, run_time);
    }

    return 0;
}