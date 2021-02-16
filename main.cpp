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
#include "doctest.h"
#include "Grid.cpp"
#include "COOGrid.cpp"
#include "FileWriter.h"
#include "learn.h"
#include "play.h"

int main(int argc, char **argv)
{
    doctest::Context context;
    // defaults: https://github.com/onqtam/doctest/blob/master/doc/markdown/main.md
    context.setOption("abort-after", 5);   // stop test execution after 5 failed assertions
    context.setOption("order-by", "name"); // sort the test cases by their name

    // add --no-run flag to run program without tests
    context.applyCommandLine(argc, argv);
    context.run();

    bool learn_or_play;
    std::cout << "Would you like to play or to learn (0 or 1)\n";
    std::cin >> learn_or_play;
    std::cout << "\n";

    if (learn_or_play)
    {
        std::cout << "About the impact of file writing (1=yes / 0=no)?\n";
        bool file_writing;
        std::cin >> file_writing;
        std::cout << "\n";
        if (file_writing)
        {

            std::cout << "What N would you like in NxN grid?\n";
            int dim;
            std::cin >> dim;

            std::cout << "How many cores would you like to use?\n";
            int n_cores;
            std::cin >> n_cores;
            file_writing_analysis(dim, n_cores);
        }

        std::cout << "About the impact of image printing (1=yes / 0=no)?\n";
        bool image_printing;
        std::cin >> image_printing;
        std::cout << "\n";
        if (image_printing)
        {
            std::cout << "What N would you like in NxN grid?\n";
            int dim;
            std::cin >> dim;

            std::cout << "How many cores would you like to use?\n";
            int n_cores;
            std::cin >> n_cores;
            image_printing_analysis(dim, n_cores);
        }

        std::cout << "About the impact of grid size in the overall performance of the code (1=yes / 0=no)?\n";
        bool grid_size;
        std::cin >> grid_size;
        std::cout << "\n";
        if (grid_size)
        {
            std::cout << "How many cores would you like to use?\n";
            int n_cores;
            std::cin >> n_cores;
        }

        std::cout << "About the impact of the number of cores in the overall performance of the code (1=yes / 0=no)?\n";
        bool cores;
        std::cin >> cores;
        std::cout << "\n";
        if (cores)
        {
            std::cout << "What N would you like in NxN grid?\n";
            int dim;
            std::cin >> dim;
        }
    }
    return 0;
}