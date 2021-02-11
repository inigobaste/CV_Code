#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

using namespace std;


int main() 
{
    bool grid_T[5][5] = {{0, 0, 0, 0, 0},{0, 1, 0, 0, 0},{0, 1, 1, 1, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0}};

    // does not store row by row so need to take transpose
    bool grid[5][5];

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            grid[i][j] = 0;
            grid[i][j] = grid_T[j][i];
            std::cout <<  grid_T[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";


    // CSR to Dense
    std::cout << "printing CSR as Dense: \n";
    int col_ind[4] = {1, 1, 2, 3};

    int row_pos[5] = {0, 0, 1, 4, 4};

    int CSRtoDense[5][5] = {{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0}};

    for (int i = 0; i < 5; i++) // # of rows
    {
        int zeroIndex = 0;

        for (int j = row_pos[i]; j < row_pos[i+1]; j++)
        {
            int k = col_ind[j];

            while (zeroIndex < k)
            {
                CSRtoDense[j][zeroIndex] = 0;
                std::cout << 0 << " ";
                zeroIndex++;
            }
            CSRtoDense[i][k] = 1;
            std::cout << 1 << " ";
            zeroIndex++;
        }
        while (zeroIndex < 5) // # of columns
        {
            CSRtoDense[i][zeroIndex] = 0;
            std::cout << 0 << " ";
            zeroIndex++;
        }
        std::cout << "\n";
    }

    // check CSR to Dense conversion
    std::cout << "\ncheck conversion: \n";
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            std::cout << CSRtoDense[i][j] << " ";
        }
        std::cout << "\n";
    }

    // Dense to CSR
    std::cout << "\nprinting Dense as CSR: \n";

    int col_ind2[4] = {0, 0, 0, 0};
    int row_pos2[5] = {0, 0, 0, 0, 0};

    int nnz_counter = 0;
    int x_store = 0;
    int y_store = 0;

    for (int i = 0; i < 5; i++) // rows 
    {
        if (nnz_counter == 0)
        {
            row_pos2[i] = 0;
        }
        else
        {
            row_pos2[i] = nnz_counter;
        }
        
        for (int j = 0; j < 5; j++) // columns
        {
            if (grid_T[i][j] != 0)
            {
                col_ind2[nnz_counter] = j;

                x_store = i;
                y_store = j;

                nnz_counter++;
            }
        }
        row_pos2[5-1] = nnz_counter;
    }



    std::cout << "col_ind = {";
    for (int i = 0; i < 4; i++)
    {
        std::cout << col_ind2[i] << " ";
    }
    std::cout << "}\n\n";

    std::cout << "row_pos = {";
    for (int i = 0; i < 5; i++)
    {
        std::cout << row_pos2[i] << " ";
    }
    std::cout << "}";
}