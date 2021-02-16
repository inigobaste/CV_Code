#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <omp.h>
#include <string>
#include "Grid.h"
#include "COOGrid.h"
#include "FileWriter.h"
void play()
{
    // Evaluate execution time for parallelised grids
    std::string par_name = "parallel_time.dat";
    std::fstream fp;
    fp.open(par_name, std::fstream::out | std::fstream::trunc);
    fp.close();

    std::cout << "How many rows would you like the grid to have?\n";
    int rows;
    std::cin >> rows;

    std::cout << "How many columns would you like the grid to have?\n";
    int cols;
    std::cin >> cols;

    std::cout << "How many cores would you like to use?\n";
    int n_cores;
    std::cin >> n_cores;

    std::cout << "How many iterations of the game would you like to play?\n";
    int its;
    std::cin >> its;

    // create a random grid
    Grid grid = Grid(rows, cols, true, n_cores);

    std::cout << "Would you like to write the data or print images of the grid (1 yes or 0 no)?\n";
    bool write_or_print;
    std::cin >> write_or_print;

    // Initialise in case output is desired at any or every iteration
    std::vector<bool> store_grids;
    std::vector<int> output_its;
    int cnt;
    bool print, write, both, all_its;

    if (write_or_print)
    {
        std::cout << "At every iteration (1 yes or 0 no)?\n";
        std::cin >> all_its;
        if (all_its)
        {
            cnt = 0;
            int choose_output;
            std::cout << "Would you like to print(0), write(1) or both(2)?\n";
            std::cin >> choose_output;

            if (choose_output == 0)
            {
                print = true;
            }
            else if (choose_output == 1)
            {
                write = true;
            }
            else
            {
                both = true;
            }

            while (its % n_cores != 0)
            {
                std::cout << "Number of iterations for the game must be a multiple of the number of cores used!\n";
                std::cout << "Provide number of iterations\n";
                std::cin >> its;
            }
        }
        else if (all_its == false)
        {
            bool add_it = true;
            int choose_its;
            std::cout << "At which iterations?\n";
            while (add_it)
            {
                std::cout << "Add iteration\n";
                std::cin >> choose_its;
                output_its.push_back(choose_its);

                std::cout << "Would you like to add further iterations (1 yes or 0 no)?\n";
                std::cin >> add_it;
            }

            int choose_output;
            std::cout << "Would you like to print(0), write(1) or both(2)?\n";
            std::cin >> choose_output;

            if (choose_output == 0)
            {
                print = true;
            }
            else if (choose_output == 1)
            {
                write = true;
            }
            else
            {
                both = true;
            }
        }
    }

    // Start clock
    start_time = omp_get_wtime();

    for (int n = 0; n < its; n++)
    {
        // Calculate next generation in the game
        grid.do_iteration();

        // Perform output operations at selected iterations
        if (output_its.size() > 0)
        {
            for (int i : output_its)
            {
                if (i == n)
                {
                    if (write)
                    {
                        grid_to_file(n, grid.cells, rows, cols);
                    }
                    else if (print)
                    {
                        print_IMG(grid.cells, rows, cols, n);
                    }
                    else if (both)
                    {
                        grid_to_file(n, grid.cells, rows, cols);
                        print_IMG(grid.cells, rows, cols, n);
                    }
                }
            }
        }

        // Perform output operations for all iterations
        if (all_its)
        {
            store_grids.resize(grid.cells.size() * n_cores);

            omp_set_num_threads(n_cores);
#pragma omp parallel for
            for (int i = cnt * grid.cells.size(); i < ((cnt + 1) * grid.cells.size()); i++)
            {
                store_grids[i] = grid.cells[i % grid.cells.size()];
            }

            // Every number of iterations equal to the number of cores used
            if ((n + 1) % n_cores == 0)
            {
                omp_set_num_threads(n_cores);
#pragma omp parallel for
                for (int i = 0; i < n_cores; i++)
                {
                    vector<bool>::const_iterator first = store_grids.begin() + i * grid.cells.size();
                    vector<bool>::const_iterator last = store_grids.begin() + (i + 1) * grid.cells.size();
                    vector<bool> v(first, last);

                    if (write)
                    {
                        grid_to_file(i + (n + 1) - n_cores, v, rows, cols);
                    }
                    else if (print)
                    {
                        print_IMG(v, rows, cols, i + (n + 1) - n_cores);
                    }
                    else if (both)
                    {
                        grid_to_file(i + (n + 1) - n_cores, v, rows, cols);
                        print_IMG(v, rows, cols, i + (n + 1) - n_cores);
                    }
                }
                cnt = -1;
                store_grids.clear();
            }
            cnt++;
        }

        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(par_name, n, rows, cols, run_time);
    }
}