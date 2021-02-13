#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <map>
#include <utility>

using namespace std;

//Note that this is a serial implementation with a periodic grid
vector<vector<bool> > grid, new_grid;
int imax, jmax;
int max_steps = 5;

int dense_to_COO(vector<int> x_coo, vector<int> y_coo) // input as output
{
    int nnzs = 0; // will store # of nnzs

    for (int i = 0; i < imax; i++)
    {
        for (int j = 0; j < jmax; j++)
        {
            if (grid[i][j]) // check for nnz entries
            {
                x_coo.push_back(i); // NOTE: might just make a pair<int> (x, y)
                y_coo.push_back(j);     // since the sizes are equal
                
                nnzs++; // NOTE: could replace with vector<int>::size_type nnzs = x_coo.size();
            }
        }
    }
    return nnzs; // maybe no need (?)
}

int num_neighbours_COO(vector<int> x_coo, vector<int> y_coo)
{
    vector<int>::size_type nnzs = x_coo.size(); // store # of nnzs

    std::vector<int> x_cooM; // store 3x3 matrix coordinates centered at nnz coords
    std::vector<int> y_cooM; // size is 9 * nnzs

    std::pair<int, int> coord_pair; // stores coordinates as pairs (x,y)

    std::multimap<std::pair<int, int>, int> indexMap; // counts repetitions of coords as value

    std::map<std::pair<int, int>, int> deathMap;

    int x, y; // temp storage

    for (int k = 0; k < nnzs; k++)
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                x_cooM.push_back(i + x_coo[k]);
                y_cooM.push_back(j + y_coo[k]);

                x = i + x_coo[k];
                y = j + y_coo[k]; 

                coord_pair = make_pair(x,y);

                indexMap.insert(make_pair(coord_pair, 1)); // could do this directly without 
                                                           // using the x_cooM & y_cooM vectors => less storage
            }
        }
    }

    for (auto itr = indexMap.begin(); itr != indexMap.end(); itr++)
    {
        deathMap.insert(make_pair(itr->first, indexMap.count(itr->first)));
    }

    int vec_it = 0; // for vector indexing
    for (auto itr = deathMap.begin(); itr != deathMap.end(); itr++) 
    {
        if (!(itr->second >= 3)) // apply rule 1 (survive/ressurect if num_neighbors = 3)
        {
            deathMap.erase(itr);

        }
        if ((itr->second == 4) && () && ()) // apply rule 1 (survive if alive & num neighbors = 4)
        {



        }
        vec_it++;
    } 



    return 0;



}





int num_neighbours(int ii, int jj)
{
    int ix, jx;
    int cnt = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
        {
            //from mxn grid to (m-1)x(n-1)
            //if (i != 0 || j != 0), SHOULD count itself as a neighbor
            
                // imax and jmax are the grid sizes
                // (ii, jj) is the current cell's coordinates
                // i & j look at the neighbors of (ii, jj)
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
    for (int i = 0; i < imax; i++)
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
    grid.swap(new_grid);
}

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

    for (int n = 0; n < max_steps; n++)
    {
        cout << "it: " << n << endl;
        do_iteration();
        grid_to_file(n);
    }

    return 0;
}
