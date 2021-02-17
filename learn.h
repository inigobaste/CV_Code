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

// This file produces execution time .dat files to evaluate the time performance
// of different operations of the game and the impact of their parallelisation

double run_time, start_time;
// This function produces execution time series when
// providing output in parallel and serial cases
void output_analysis(int dim, int n_cores, bool write_or_print)
{
    // Number of generations in the game must be a multiple of
    // the number of cores, due to the way output operations
    // are parallelised
    int max_steps = n_cores * 100;

    // Initialise file to write execution time for parallelised output
    std::string par_name;
    if (write_or_print)
    {
        par_name = "parallel_FW_time.dat";
    }
    else
    {
        par_name = "parallel_IP_time.dat";
    }
    std::fstream fp;
    fp.open(par_name, std::fstream::out | std::fstream::trunc);
    fp.close();

    // create a random grid
    Grid grid = Grid(dim, dim, true, n_cores);

    // Vector containing n_cores number of grids
    // from previous iterations gets initialised
    std::vector<bool> store_grids;
    store_grids.resize(grid.cells.size() * n_cores);

    // Counter initialised. Indicates iterations after last output
    int cnt = 0;

    // Start clock
    start_time = omp_get_wtime();

    for (int n = 0; n < max_steps; n++)
    {
        // Calculate next generation in the game
        grid.do_iteration();

        // Assign grid of current iteration to vector of
        // previous iterations
        omp_set_num_threads(n_cores);
#pragma omp parallel for
        for (int i = cnt * grid.cells.size(); i < ((cnt + 1) * grid.cells.size()); i++)
        {
            store_grids[i] = grid.cells[i % grid.cells.size()];
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
                // Each thread takes one grid from a previous iteration
                vector<bool>::const_iterator first = store_grids.begin() + i * grid.cells.size();
                vector<bool>::const_iterator last = store_grids.begin() + (i + 1) * grid.cells.size();
                vector<bool> v(first, last);

                // Depending on option chosen, write grid
                // to .dat, or print .bmp image
                if (write_or_print)
                {
                    grid_to_file(i + (n + 1) - n_cores, v, dim, dim);
                }
                else
                {
                    print_IMG(v, dim, dim, i + (n + 1) - n_cores);
                }
            }
            // Restart counter
            cnt = -1;
        }
        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(par_name, n, dim, dim, run_time);
        cnt++;
    }

    // Initialise file to write execution time for serial output
    std::string ser_name;
    if (write_or_print)
    {
        ser_name = "serial_FW_time.dat";
    }
    else
    {
        ser_name = "serial_IP_time.dat";
    }

    std::fstream fs;
    fs.open(ser_name, std::fstream::out | std::fstream::trunc);
    fs.close();

    // Start clock
    start_time = omp_get_wtime();

    for (int n = 0; n < max_steps; n++)
    {
        // Calculate next generation in the game
        grid.do_iteration();

        // Depending on option chosen, write grid
        // to .dat, or print .bmp image
        if (write_or_print)
        {
            grid.to_file(n);
        }
        else
        {
            print_IMG(grid.cells, dim, dim, n);
        }

        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(ser_name, n, dim, dim, run_time);
    }
}

// This function produces execution time series when
// for different sizes of serialised and parallelised
// grids
void size_analysis(int n_cores)
{
    // Evaluate execution time for parallelised grids
    std::string par_name = "parallel_size_time.dat";
    std::fstream fp;
    fp.open(par_name, std::fstream::out | std::fstream::trunc);
    fp.close();

    std::vector<int> dims = {10, 100, 1000, 5000, 10000, 20000};

    // Start clock
    start_time = omp_get_wtime();

    for (int dim : dims)
    {
        // Create a random grid of different size for each iteration
        Grid grid = Grid(dim, dim, true, n_cores);

        // Calculate next generation in the game
        grid.do_iteration();

        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(par_name, 1, dim, dim, run_time);
    }

    // Evaluate execution time for serialised grids
    std::string ser_name = "serial_size_time.dat";

    std::fstream fs;
    fs.open(ser_name, std::fstream::out | std::fstream::trunc);
    fs.close();

    int single_core = 1;

    // Start clock
    start_time = omp_get_wtime();

    for (int dim : dims)
    {
        // Generate random NxN grid
        Grid grid = Grid(dim, dim, false, single_core);

        // Calculate next generation in the game
        grid.do_iteration();

        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(ser_name, 1, dim, dim, run_time);
    }
}

void cores_analysis(int dim)
{
    // Evaluate execution time for parallelised grids
    std::string par_name = "parallel_cores_time.dat";
    std::fstream fp;
    fp.open(par_name, std::fstream::out | std::fstream::trunc);
    fp.close();

    std::vector<int> cores = {1, 2, 4, 8, 12};

    for (int n_cores : cores)
    {
        // Start clock
        start_time = omp_get_wtime();

        // create a random grid
        Grid grid = Grid(dim, dim, true, n_cores);

        // Calculate next generation in the game
        grid.do_iteration();

        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(par_name, n_cores, dim, dim, run_time);
    }
}