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
// #include "Grid.cpp"
#include "COOGrid.h"
// #include "COOGrid.cpp"
#include "FileWriter.h"

double run_time, start_time;

// This function calculates execution time series
// for parallelisation of file-writing and for the
// serial case
void file_writing_analysis(int dim, int n_cores)
{
    // Number of generations in the game
    int max_steps = n_cores * 100;

    // Evaluate execution time for parallelised file-writing
    std::string par_name = "parallel_FW_time.dat";
    std::fstream fp;
    fp.open(par_name, std::fstream::out | std::fstream::trunc);
    fp.close();

    if (dim > 10000)
    {
        dim = 10000;
        std::cout << "Your size input was too large, we use N=1000 instead\n";
    }

    if (dim < 100)
    {
        dim = 10000;
        std::cout << "Your size input was too small, we use N=1000 instead\n";
    }

    // create a random grid
    Grid grid = Grid(dim, dim, true);

    start_time = omp_get_wtime();

    std::vector<bool> store_grids;

    int cnt = 0;

    for (int n = 0; n < max_steps; n++)
    {
        // Calculate next generation in the game
        grid.do_iteration();
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

                grid_to_file(i + (n + 1) - n_cores, v, dim);
            }
            cnt = -1;
            store_grids.clear();
        }
        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(par_name, n, dim, run_time);
        cnt++;
    }

    start_time = omp_get_wtime();

    // Evaluate execution time for serial file-writing
    std::string ser_name = "serial_FW_time.dat";
    std::fstream fs;
    fs.open(ser_name, std::fstream::out | std::fstream::trunc);
    fs.close();

    for (int n = 0; n < max_steps; n++)
    {
        // Calculate next generation in the game
        grid.do_iteration();
        grid.to_file(n);

        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(ser_name, n, dim, run_time);
    }
}

// This function calculates execution time series
// for parallelisation of image-printing and for the
// serial case
void image_printing_analysis(int dim, int n_cores)
{
    // Number of generations in the game
    int max_steps = n_cores * 100;

    // Evaluate execution time for parallelised file-writing
    std::string par_name = "parallel_IP_time.dat";
    std::fstream fp;
    fp.open(par_name, std::fstream::out | std::fstream::trunc);
    fp.close();

    if (dim > 10000)
    {
        dim = 10000;
        std::cout << "Your size input was too large, we use N=1000 instead\n";
    }

    if (dim < 100)
    {
        dim = 10000;
        std::cout << "Your size input was too small, we use N=1000 instead\n";
    }

    // create a random grid
    Grid grid = Grid(dim, dim, true);

    start_time = omp_get_wtime();

    std::vector<bool> store_grids;

    int cnt = 0;

    for (int n = 0; n < max_steps; n++)
    {
        // Calculate next generation in the game
        grid.do_iteration();
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

                print_IMG(v, dim, dim, i + (n + 1) - n_cores);
            }
            cnt = -1;
            store_grids.clear();
        }
        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(par_name, n, dim, run_time);
        cnt++;
    }

    // Evaluate execution time for serial file-writing
    std::string ser_name = "serial_IP_time.dat";
    std::fstream fs;
    fs.open(ser_name, std::fstream::out | std::fstream::trunc);

    for (int n = 0; n < max_steps; n++)
    {
        // Calculate next generation in the game
        grid.do_iteration();

        print_IMG(grid.cells, dim, dim, n);

        // Record time at time-step
        run_time = omp_get_wtime() - start_time;
        time_data_to_file(ser_name, n, dim, run_time);
    }
}

//     vector<int> dims{1000};

//     for (int dim : dims)
//     {
//         std::cout << "Dimension: " << dim << "\n";
//         // create a random grid
//         Grid grid = Grid(dim, dim, true);

//         start_time = omp_get_wtime();
//         int threads = 12;

//         std::vector<bool> store_grids;
//         int ID;
//         int cnt = 0;
//         std::cout << grid.cells.size() << std::endl;

//         for (int n = 0; n < max_steps; n++)
//         {
//             grid.do_iteration();
//             // store_grids.resize(grid.cells.size() * threads);
//             grid.to_file(n);

// #pragma omp parallel for
//             for (int i = cnt * grid.cells.size(); i < ((cnt + 1) * grid.cells.size()); i++)
//             {
//                 store_grids[i] = grid.cells[i % grid.cells.size()];
//             }

//             if ((n + 1) % threads == 0)
//             {
// #pragma omp parallel for
//                 for (int i = 0; i < threads; i++)
//                 {
//                     vector<bool>::const_iterator first = store_grids.begin() + i * grid.cells.size();
//                     vector<bool>::const_iterator last = store_grids.begin() + (i + 1) * grid.cells.size();
//                     vector<bool> v(first, last);

//                     grid_to_file(i + (n + 1) - threads, v, dim);
//                     std::stringstream fname;
//                     fname << "IMG"
//                           << "_" << i + (n + 1) - threads << ".bmp";
//                     writeGridImageFile(v, fname.str(), dim, dim);
//                 }
//                 cnt = -1;
//                 store_grids.clear();
//             }

//             run_time = omp_get_wtime() - start_time;
//             grid.time_data_to_file(n, dim, run_time);
//             cnt++;
//         }
//     }

//     return 0;
// }

// std::stringstream fname;
//                 fname << "IMG"
//                       << "_" << i + (n + 1) - n_cores << ".bmp";
//                 writeGridImageFile(v, fname.str(), dim, dim);