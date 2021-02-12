#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <omp.h>
#include <string>

using namespace std;

//Note that this is a serial implementation with a periodic grid
vector<bool> grid, new_grid;
int counter = 0;
int imax, jmax;
int max_steps = 1;

int num_neighbours(int ii, int jj)
{
    int ix, jx;
    int cnt = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (i != 0 || j != 0)
            {
                ix = (i + ii + imax) % imax;
                jx = (j + jj + jmax) % jmax;
                if (grid[ix * imax + jx])
                    cnt++;
            }
    return cnt;
}

void grid_to_file(int it)
{
    stringstream fname;
    fstream f1;
    fname << "output"
          << "_" << it << ".dat";
    f1.open(fname.str().c_str(), ios_base::out);
    for (int i = 0; i < imax; i++)
    {
        for (int j = 0; j < jmax; j++)
            f1 << grid[i * imax + j] << "\t";
        f1 << endl;
    }
    f1.close();
}

void do_iteration()
{

#pragma omp parallel for
    for (int i = 0; i < grid.size(); i++)
    {
        new_grid[i] = grid[i];
    }

#pragma omp parallel for

    for (int i = 0; i < imax; i++)
    {

        for (int j = 0; j < jmax; j++)
        {

            int num_n = num_neighbours(i, j);
            if (grid[i * imax + j])
            {
                if (num_n != 2 && num_n != 3)
                    new_grid[i * imax + j] = false;
            }
            else if (num_n == 3)
                new_grid[i * imax + j] = true;
        }
    }

    // threading ends here
    grid.swap(new_grid);
}

void time_data_to_file(int steps, int size, double time)
{
    string fname = "time_data.dat";
    ofstream f1;
    f1.open(fname, std::ofstream::app);

    if (f1.is_open())
    {
        f1 << size << "\t" << steps << "\t" << time;
        f1 << endl;
    }

    f1.close();
}

int num_threads = 10;
double run_time, start_time;
int main(int argc, char *argv[])
{
    ofstream ofs;
    ofs.open("time_data.dat", std::ofstream::trunc);
    ofs.close();
    vector<int> dims{10, 20, 100, 500, 1000, 5000, 10000};
    cout << dims.size() << endl;
    for (int dim : dims)
    {
        grid.clear();
        new_grid.clear();
        imax = dim;
        jmax = dim;

        start_time = omp_get_wtime();
        srand(time(NULL));

        // Parallel initialisation
        //set an initial random collection of points - You could set an initial pattern
        grid.resize(imax * jmax);
#pragma omp parallel for
        for (int i = 0; i < grid.size(); i++)
            grid[i] = (rand() % 2);

        new_grid.resize(grid.size());

        cout << "size: " << grid.size() << endl;
        cout << "imax: " << imax << endl;

        for (int n = 0; n < max_steps; n++)
        {
            cout << "it: " << n << "k: " << dim << endl;
            do_iteration();
            cout << "write to file\n";
            grid_to_file(n);
        }

        run_time = omp_get_wtime() - start_time;
        time_data_to_file(max_steps, imax, run_time);
    }

    return 0;
}
