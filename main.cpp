#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <omp.h>
#include <string>
#include "Grid.h"

using namespace std;

double run_time, start_time;
int max_steps = 1;

int main()
{
    ofstream ofs;
    ofs.open("time_data.dat", std::ofstream::trunc);
    ofs.close();

    vector<int> dims{10, 20, 100, 500, 1000, 5000};

    for (int dim : dims)
    {
        cout << "Dimension: " << dim << endl;
        // create a random grid
        Grid grid = Grid(dim, dim, true);

        start_time = omp_get_wtime();
        srand(time(NULL));

        for (int n = 0; n < max_steps; n++)
        {
            grid.do_iteration();
            grid.to_file(n);
        }

        run_time = omp_get_wtime() - start_time;
        grid.time_data_to_file(max_steps, dim, run_time);
    }

    return 0;
}