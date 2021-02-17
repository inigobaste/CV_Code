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

// This file prompts the user to give input, so they can make some adjustments
// to the game in terms of grid characteristics and output of game iterations
void play()
{
    // Initialise file to write execution time for parallelised grids
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

    std::cout << "How many cores would you like to use? Please enter a number less than or equal to the number of logical cores in your CPU.\n";
    int n_cores;
    std::cin >> n_cores;

    std::cout << "How many iterations of the game would you like to play?\n";
    int its;
    std::cin >> its;

    // Create a random grid
    Grid grid = Grid(rows, cols, true, n_cores);

    std::cout << "Would you like some kind of output (1 yes or 0 no)?\n";
    bool write_or_print;
    std::cin >> write_or_print;

    // Initialise in case output is desired at any or every iteration
    std::vector<bool> store_grids;
    std::vector<std::string> string_grids;
    std::vector<int> output_its;
    int cnt;
    bool print = false;
    bool write = false;
    bool both = false;
    bool all_its = false;

    if (write_or_print)
    {
        std::cout << "For every iteration (1 yes or 0 no)?\n";
        std::cin >> all_its;
        if (all_its)
        {
            cnt = 0;
            int choose_output;
            std::cout << "Would you like to print images(0), write data(1) or both(2)?\n";
            std::cin >> choose_output;

            // Set booleans for different output options
            if (choose_output == 0)
            {
                print = true;
                store_grids.resize(grid.cells.size() * n_cores);
            }
            else if (choose_output == 1)
            {
                write = true;
                string_grids.resize(n_cores);
            }
            else
            {
                both = true;
                // Vector containing n_cores number of grids
                // from previous iterations gets initialised
                string_grids.resize(n_cores);
                store_grids.resize(grid.cells.size() * n_cores);
            }

            // Because of the particular way in which we have decided to parallelise our output
            // (image printing and file writing), where printing occurs every n_core number of
            // iterations occur, the number of iterations must be a multiple of the number of cores used.
            while (its % n_cores != 0)
            {
                std::cout << "Number of iterations for the game must be a multiple of the number of cores used!\n";
                std::cout << "Provide number of iterations\n";
                std::cin >> its;
            }
        }
        else if (all_its == false)
        {
            // Decide iterations for which output is produced
            bool add_it = true;
            int choose_its;
            std::cout << "For which iterations?\n";
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
    std::cout << "Please wait...\n";

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
                    // Depending on option chosen, write grid
                    // to .dat, print .bmp image, or both
                    if (write)
                    {
                        grid_to_file(n, grid.data, rows, cols);
                    }
                    else if (print)
                    {
                        print_IMG(grid.cells, rows, cols, n);
                    }
                    else if (both)
                    {
                        grid_to_file(n, grid.data, rows, cols);
                        print_IMG(grid.cells, rows, cols, n);
                    }
                }
            }
        }

        // Perform output operations for all iterations
        if (all_its)
        {

            if (write)
            {
                string_grids[cnt] = grid.data;
            }
            else if (print)
            {
                // Assign grid of current iteration to vector of
                // previous iterations
                omp_set_num_threads(n_cores);
#pragma omp parallel for
                for (int i = cnt * grid.cells.size(); i < ((cnt + 1) * grid.cells.size()); i++)
                {
                    store_grids[i] = grid.cells[i % grid.cells.size()];
                }
            }
            else if (both)
            {
                // Assign grid of current iteration to vector of
                // previous iterations
                omp_set_num_threads(n_cores);
#pragma omp parallel for
                for (int i = cnt * grid.cells.size(); i < ((cnt + 1) * grid.cells.size()); i++)
                {
                    store_grids[i] = grid.cells[i % grid.cells.size()];
                }
                string_grids[cnt] = grid.data;
            }

            // Every number of iterations equal to the number of cores used
            if ((n + 1) % n_cores == 0)
            {
                // Each thread will perform output operations on a previously
                // stored iteration of the grid
                omp_set_num_threads(n_cores);
#pragma omp parallel for
                for (int i = 0; i < n_cores; i++)
                {

                    // Depending on option chosen, write grid
                    // to .dat, print .bmp image, or both
                    if (write)
                    {
                        grid_to_file(i + (n + 1) - n_cores, string_grids[i], rows, cols);
                    }
                    else if (print)
                    {
                        vector<bool>::const_iterator first = store_grids.begin() + i * grid.cells.size();
                        vector<bool>::const_iterator last = store_grids.begin() + (i + 1) * grid.cells.size();
                        vector<bool> v(first, last);
                        print_IMG(v, rows, cols, i + (n + 1) - n_cores);
                    }
                    else if (both)
                    {
                        vector<bool>::const_iterator first = store_grids.begin() + i * grid.cells.size();
                        vector<bool>::const_iterator last = store_grids.begin() + (i + 1) * grid.cells.size();
                        vector<bool> v(first, last);
                        grid_to_file(i + (n + 1) - n_cores, string_grids[i], rows, cols);
                        print_IMG(v, rows, cols, i + (n + 1) - n_cores);
                    }
                }
                // Restart counter
                cnt = -1;
            }
            cnt++;
        }

        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(par_name, n, rows, cols, run_time);
    }
    std::cout << "Execution complete. Your files are now available." << std::endl;
}