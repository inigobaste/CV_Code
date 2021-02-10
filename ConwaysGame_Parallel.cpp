#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <omp.h>

using namespace std;

//Note that this is a serial implementation with a periodic grid
vector<vector<bool>> grid, new_grid;
int counter = 0;
int imax, jmax;
int max_steps = 5;

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
                if (grid[ix][jx])
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
            f1 << grid[i][j] << "\t";
        f1 << endl;
    }
    f1.close();
}

void do_iteration(void)
{
    int nthrds;
    int ID = omp_get_thread_num();
    nthrds = omp_get_num_threads();
#pragma omp parallel
    {

        for (int i = ID; i < imax; i += nthrds)
            for (int j = 0; j < jmax; j++)
            {
                new_grid[i][j] = grid[i][j];
                int num_n = num_neighbours(i, j);
                if (grid[i][j])
                {
                    if (num_n != 2 && num_n != 3)
                        new_grid[i][j] = false;
                }
                else if (num_n == 3)
                    new_grid[i][j] = true;
            }
    }
    // threading ends here
    grid.swap(new_grid);
}

int num_threads = 4;
int main(int argc, char *argv[])
{
    srand(time(NULL));
    imax = 8;
    jmax = 8;
    grid.resize(imax, vector<bool>(jmax));
    new_grid.resize(imax, vector<bool>(jmax));

    //set an initial random collection of points - You could set an initial pattern
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++)
            grid[i][j] = (rand() % 2);

    // Parallelising
    // int nthreads;
    // omp_set_num_threads(num_threads);
    // #pragma omp parallel
    //     {
    //         int nthrds;
    //         int ID = omp_get_thread_num();
    //         nthrds = omp_get_num_threads();

    //         //Check that you are in the correct thread
    //         if (ID == 0)
    //             nthreads = nthrds;

    for (int n = 0; n < max_steps; n++)
    {
        cout << "it: " << n << endl;
        do_iteration();
        grid_to_file(n);
    }
    // }

    return 0;
}
