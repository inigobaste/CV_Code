#include <cstdint> // for specific size integers
#include <fstream> // for file handling
#include <vector>
#include <memory>
#include <stdio.h>

using namespace std;

void writeGridImageFile(const std::vector<bool> &grid, std::string filename, int w, int h)
{
    FILE *f;
    unsigned char *img = new unsigned char[3 * w * h];
    // std::unique_ptr<unsigned char[]> img = std::unique_ptr<unsigned char[]>(new unsigned char[3 * w * h]);

    int filesize = 54 + 3 * w * h; // w is image width, h is image height

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            int x = i;
            int y = (h - 1) - j;

            int rand_grey = rand() % 20 + 230;
            int r = rand_grey;
            int g = rand_grey;
            int b = rand_grey;

            if (grid[i * w + j])
            {
                r = rand() % 55 + 200; // random red
                g = 0;
                b = 0;
            }

            img[(x + y * w) * 3 + 2] = (unsigned char)(r);
            img[(x + y * w) * 3 + 1] = (unsigned char)(g);
            img[(x + y * w) * 3 + 0] = (unsigned char)(b);
        }
    }

    // 14 bytes in total (14 unsigned chars)
    unsigned char bmpfileheader[14] = {
        'B', 'M',   // signature
        0, 0, 0, 0, // size of file in bytes
        0, 0, 0, 0,
        54, 0, 0, 0 // offset
    };

    unsigned char bmpinfoheader[40] = {
        40, 0, 0, 0,  // size
        0, 0, 0, 0,   // width
        0, 0, 0, 0,   // height
        1, 0,         // planes
        24, 0,        // bits per pixel
        0, 0, 0, 0,   // compression: no compression for now
        0, 0, 0, 0,   // compressed size of image: 0 because no compression
        100, 0, 0, 0, // vertical resolution
        100, 0, 0, 0  // horizontal resolution
    };
    unsigned char bmppad[3] = {0, 0, 0};

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4] = (unsigned char)(w);
    bmpinfoheader[5] = (unsigned char)(w >> 8);
    bmpinfoheader[6] = (unsigned char)(w >> 16);
    bmpinfoheader[7] = (unsigned char)(w >> 24);
    bmpinfoheader[8] = (unsigned char)(h);
    bmpinfoheader[9] = (unsigned char)(h >> 8);
    bmpinfoheader[10] = (unsigned char)(h >> 16);
    bmpinfoheader[11] = (unsigned char)(h >> 24);

    f = fopen(filename.c_str(), "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    for (int i = 0; i < h; i++)
    {
        fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
        fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
    }

    fclose(f);

    delete[] img;
}