#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <omp.h>
#include <string>
#include "doctest.h"
#include "learn.h"
#include "play.h"

// This file gives the user the option to test, play or receive output about the code performance
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
    std::cout << "Would you like to play or to learn (0=play or 1=learn)\n";
    std::cin >> learn_or_play;
    std::cout << "\n";

    if (learn_or_play)
    {

        std::cout << "About the impact of file writing or image printing (1=yes / 0=no)?\n";
        bool output;
        std::cin >> output;
        std::cout << "\n";
        if (output)
        {

            std::cout << "Choose file writing (1) or image printing (0)?\n";
            bool write_or_print;
            std::cin >> write_or_print;

            std::cout << "What N would you like in NxN grid?\n";
            int dim;
            std::cin >> dim;

            std::cout << "How many cores would you like to use? Please enter a number less than or equal to the number of logical cores in your CPU.\n";
            int n_cores;
            std::cin >> n_cores;

            std::cout << "Code running...\n";
            output_analysis(dim, n_cores, write_or_print);
        }

        std::cout << "About the impact of grid size in the performance of the game (1=yes / 0=no)?\n";
        bool grid_size;
        std::cin >> grid_size;
        std::cout << "\n";
        if (grid_size)
        {
            std::cout << "How many cores would you like to use? Please enter a number less than or equal to the number of logical cores in your CPU.\n";
            int n_cores;
            std::cin >> n_cores;
            std::cout << "Code running...\n";
            size_analysis(n_cores);
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
            std::cout << "Code running...\n";
            cores_analysis(dim);
        }
    }

    else if (learn_or_play == false)
    {
        play();
    }

    return 0;
}