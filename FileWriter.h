#pragma once
#include <cstdint>
#include <fstream>
#include <vector>
#include <memory>
#include <stdio.h>
#include <string>
#include "Grid.h"

using namespace std;

void write_grid_image_file(const std::vector<bool> &grid, std::string filename, int w, int h)
{
    FILE *f;

    // unsigned char has 255 options (one byte), we need three bytes per pixel/cell
    unsigned char *img = new unsigned char[3 * w * h];

    // w is number of rows, h number of columns
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            int rand_grey = rand() % 20 + 230;
            int r = rand_grey;
            int g = rand_grey;
            int b = rand_grey;

            if (grid[i * w + j])
            {
                // create a random red
                r = rand() % 55 + 200;
                g = 0;
                b = 0;
            }

            // add 3 bytes for red, green and blue
            img[(j + i * w) * 3 + 2] = (unsigned char)(r);
            img[(j + i * w) * 3 + 1] = (unsigned char)(g);
            img[(j + i * w) * 3 + 0] = (unsigned char)(b);
        }
    }

    // size in bytes
    int fileHeaderSize = 14;
    int infoHeaderSize = 40;

    int headerSize = fileHeaderSize + infoHeaderSize;
    int filesize = headerSize + w * h * 3;

    // 14 bytes in total (14 unsigned chars)
    unsigned char bmpFileHeader[14] = {
        'B', 'M',   // signature
        0, 0, 0, 0, // size of file in bytes - populate later
        0, 0, 0, 0, // reserved
        54, 0, 0, 0 // header offset
    };

    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,  // size
        0, 0, 0, 0,   // width - populate later
        0, 0, 0, 0,   // height - populate later
        1, 0,         // planes
        24, 0,        // bits per pixel - RGB requires 3 bytes (24 bits)
        0, 0, 0, 0,   // compression: no compression for now
        0, 0, 0, 0,   // compressed size of image: 0 because no compression
        100, 0, 0, 0, // vertical resolution in pixels/m
        100, 0, 0, 0  // horizontal resolution
    };
    unsigned char bmppad[3] = {0, 0, 0};

    // since filesize might require more than one byte memory, we use
    // the >> bitwise right-shift operator to populate the chars
    bmpFileHeader[2] = (unsigned char)(filesize);
    bmpFileHeader[3] = (unsigned char)(filesize >> 8);
    bmpFileHeader[4] = (unsigned char)(filesize >> 16);
    bmpFileHeader[5] = (unsigned char)(filesize >> 24);

    // do the same for width and height
    bmpInfoHeader[4] = (unsigned char)(w);
    bmpInfoHeader[5] = (unsigned char)(w >> 8);
    bmpInfoHeader[6] = (unsigned char)(w >> 16);
    bmpInfoHeader[7] = (unsigned char)(w >> 24);
    bmpInfoHeader[8] = (unsigned char)(h);
    bmpInfoHeader[9] = (unsigned char)(h >> 8);
    bmpInfoHeader[10] = (unsigned char)(h >> 16);
    bmpInfoHeader[11] = (unsigned char)(h >> 24);

    f = fopen(filename.c_str(), "wb");

    fwrite(bmpFileHeader, 1, 14, f);
    fwrite(bmpInfoHeader, 1, 40, f);

    for (int i = 0; i < h; i++)
    {
        fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
        fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
    }

    fclose(f);

    delete[] img;
}

void grid_to_file(int it, std::string grid_string, int rows, int cols)
{
    std::stringstream fname;
    std::fstream f1;

    fname << "run"
          << "_" << it << ".dat";

    f1.open(fname.str().c_str(), std::ios_base::out);

    f1 << grid_string;
    f1.close();
}

void print_IMG(std::vector<bool> grid_input, int rows, int cols, int it)
{
    std::stringstream fname;
    fname << "ParallelPanicking_gen-"
          << it << "_" << rows << "x" << cols << ".bmp";
    write_grid_image_file(grid_input, fname.str(), rows, cols);
}

void time_data_to_file(std::string fname, int steps, int rows, int cols, double time)
{
    std::fstream f;
    f.open(fname, std::fstream::out | std::fstream::app);

    if (f.is_open())
    {
        f << rows << "\t" << cols << "\t" << steps << "\t" << time;
        f << std::endl;
    }

    f.close();
}