#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <omp.h>
#include "FileWriter.h"

using namespace std;

//Note that this is a serial implementation with a periodic grid
vector<bool> grid, new_grid;
int imax, jmax;
int max_steps = 1;

int num_neighbours(int ii, int jj)
{
    int ix, jx;
    int cnt = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            //from mxn grid to (m-1)x(n-1)
            if (i != 0 || j != 0) // shouldn't count itself as a neighbor
            {
                // imax and jmax are the grid sizes
                // (ii, jj) is the current cell's coordinates
                // i & j look at the neighbors of (ii, jj)
                ix = (i + ii + imax) % imax;
                jx = (j + jj + jmax) % jmax;
                if (grid[ix * jmax + jx])
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
            f1 << grid[i * jmax + j] << "\t";
        f1 << endl;
    }
    f1.close();
}

void do_iteration(void)
{
    for (int i = 0; i < grid.size(); i++)
    {
        new_grid.push_back(grid[i]);
    }
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++)
        {

            int num_n = num_neighbours(i, j);
            if (grid[i * jmax + j])
            {
                if (num_n != 2 && num_n != 3)
                    new_grid[i * jmax + j] = false;
            }
            else if (num_n == 3)
                new_grid[i * jmax + j] = true;
        }
    grid.swap(new_grid);
}

void time_data_to_file(int steps, int size, double time)
{
    string fname = "time_data_serial.dat";
    ofstream f1;
    f1.open(fname, std::ofstream::app);

    if (f1.is_open())
    {
        f1 << size << "\t" << steps << "\t" << time;
        f1 << endl;
    }

    f1.close();
}

double run_time, start_time;
int main(int argc, char *argv[])
{
    ofstream ofs;
    ofs.open("time_data_serial.dat", std::ofstream::trunc);
    ofs.close();
    vector<int> dims{4, 10, 20, 100, 1000, 5000, 10000};
    for (int dim : dims)
    {
        grid.clear();
        new_grid.clear();
        srand(time(NULL));
        imax = dim;
        jmax = dim;
        start_time = omp_get_wtime();

        //set an initial random collection of points - You could set an initial pattern
        for (int i = 0; i < imax; i++)
            for (int j = 0; j < jmax; j++)
                grid.push_back(rand() % 2);

        for (int n = 0; n < max_steps; n++)
        {
            cout << "it: " << n << endl;
            do_iteration();
            grid_to_file(n);
        }
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(max_steps, imax, run_time);
    }

    writeGridImageFile(grid, "parallel_grid.bmp", jmax, imax);

    return 0;
}
