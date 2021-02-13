#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

using namespace std;

#include <unordered_map>
#include <map>
#include <utility> // for std::pair

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

    //int col_ind2[4] = {0, 0, 0, 0};
    //int row_pos2[5] = {0, 0, 0, 0, 0};

    int grids[4][5] = {{1, 0, 0, 1, 0},{0, 0, 1, 0, 1},{0, 1, 0, 0, 0},{0, 0, 1, 0, 1}};

    int col_ind2[7] = {0, 0, 0, 0, 0, 0, 0};
    int row_pos2[5] = {0, 0, 0, 0, 0};

    int nnz_counter = 0;
    int x_store = 0;
    int y_store = 0;

    for (int i = 0; i < 4; i++) // rows 
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
            if (grids[i][j] != 0)
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
    for (int i = 0; i < 7; i++)
    {
        std::cout << col_ind2[i] << " ";
    }
    std::cout << "}\n\n";

    std::cout << "row_pos = {";
    for (int i = 0; i < 5; i++)
    {
        std::cout << row_pos2[i] << " ";
    }
    std::cout << "}\n\n";


    // Dense to COO conversion
        // 
    int grid_T2[5][5] = {{0, 0, 0, 0, 0},{0, 1, 0, 0, 0},{0, 1, 1, 1, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0}};

    std::vector<int> x_coo; // store nnz coordinates
    std::vector<int> y_coo; // size is nnzs

    int nnzs = 0;

    std::vector<int> x_cooM; // store 3x3 matrix coordinates centered at nnz coords
    std::vector<int> y_cooM; // size is 9 * nnzs

    std::pair<int, int> coord_pair; // stores coordinates as pairs (x,y)
    int x, y;

    std::multimap<std::pair<int, int>, int> indexMap; // counts repetitions of coords as value

    std::map<std::pair<int, int>, int> deathMap;

    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (grid_T2[i][j] != 0)
            {
                nnzs++;
                x_coo.push_back(i);
                y_coo.push_back(j);

                // store the 3x3 matrices, should be outside as a seperate fxn
                for (int k = -1; k <= 1; k++)
                {
                    for (int m = -1; m <= 1; m++)
                    {
                        x_cooM.push_back(i + k);
                        y_cooM.push_back(j + m);

                        x = i + k;
                        y = j + m;

                        coord_pair = make_pair(x,y);
                        indexMap.insert(make_pair(coord_pair, 1 /*+ indexMap.count(coord_pair)*/));
                    }
                }
                // Rules:
                    // 1- alive/survive if repeating 3 times
                    // 2- survive if repeating 4 times

                // for each coordinate (x_coo, y_coo) create a 3x3 matrix centered at (x_coo, y_coo)
                // look at how many times each element repeats, 9 * nnzs
            }   // look at "pair", counting in "unordered map"
        }
    }

    //////// Unimportant
    std::cout << "\nAlive cell coordinates :\n";
    for (int i = 0; i < nnzs; i++)
    {
        std::cout << "(" << x_coo[i] << ", " << y_coo[i] << ")\n";
    }
    ////////////////////


    for (auto itr = indexMap.begin(); itr != indexMap.end(); itr++)
    {
        deathMap.insert(make_pair(itr->first, indexMap.count(itr->first)));
    }



/*
    int rep = 0; // count repetitions
    for (auto itr = indexMap.begin(); itr != indexMap.end(); itr++)  
    {
        if (itr -> first == make_pair(x_cooM[rep], y_cooM[rep]))
        {
            itr -> second++;

            deathMap.insert(make_pair(coord_pair, rep));
        }
        rep++;
        std::cout << "Rep: " << rep << " ";
    } */

    vector<int> x_coo_; // to add to original matrix
    vector<int> y_coo_;

    std::cout << "COO storage:\n(x, y):\n";

    for (int i = 0; i < nnzs; i++)
    {
        //std::cout << x_coo[i] << ", " << y_coo[i] << "\n";
    }

    std::cout << x_cooM.size() << "  " << y_cooM.size() << "\n";

    for (int i = 0; i < x_cooM.size(); i++)
    {
        std::cout << x_cooM[i] << ", " << y_cooM[i] << "\n";
    }

    std::cout << "     " << coord_pair.first << " " << coord_pair.second << "\n\n\n";

    int jk = 0;
    for (auto itr = deathMap.begin(); itr != deathMap.end(); itr++) { 
        std::cout << "(" << itr->first.first << ", " << itr->first.second << ") = " << itr->second << "   " << jk << '\n'; 
        jk++;
    } 

    std::map<std::pair<int, int>, int> deathMap2;
    
    //////////////////////////
    // COULD BE ITS OWN FXN // // Version for ONLY 1 Storage Map
    //////////////////////////
    int itr_int = 0;
    for (auto itr = deathMap.begin(); itr != deathMap.end();) 
    {
        if ((itr->second < 3) || (itr->second > 4))  //Rule 1 implemented, Rule No 2 --> || (!(() && () && ()))
        {
            //deathMap.erase(make_pair(itr->first.first, itr->first.second));
            //itr = deathMap[itr->first];
            
            deathMap.erase(itr);
            //itr = deathMap.end();
            itr--;
            itr_int--;

            if (itr_int < 0) // this is for satisfying the edge case 
            {                // where we have many (chain) entries which have value < 3
                itr = deathMap.begin(); 
                itr_int = 0;
            }

            //deathMap2.insert(make_pair(itr->first.first, itr->first.second),itr->second); 
            // 0,0 index fucked    
        }
        else if (itr->second == 4) // Rule 2
        {
            int i = 0;
            bool survivor = false; // look if 4 times repeated coord was alive previously
            bool s1 = false;
            bool s2 = false;
            while ((i < nnzs) && !(survivor == false)) // check if survivor
            {
                s1 = (itr->first.first == x_coo[i]);
                s2 = (itr->first.second == y_coo[i]);

                std::cout << "s1 = " << s1 << ",  s2 = " << s2 << ",  survivor = " << survivor << "\n";

                survivor = (s1 && s2);
                i++;
            }

            survivor = survivor;

            if (survivor == false)
            {
                deathMap.erase(itr);
                itr--;
                itr_int--;

                if (itr_int < 0) 
                {               
                    itr = deathMap.begin(); 
                    itr_int = 0;
                }
            }
        }
        else
        {
            itr++;
            itr_int++;
        }
        
        //itr++;
    } 

    std::cout << "\nMap Size = " << deathMap.size();

    //deathMap.erase(make_pair(0,1));

    std::cout << "\nUpdated:\n";
    jk = 0;
    for (auto itr = deathMap.begin(); itr != deathMap.end(); itr++) { 
        std::cout << "(" << itr->first.first << ", " << itr->first.second << ") = " << itr->second << "   " << jk << '\n'; 
        jk++;
    } 



    //vector<int> x_temp(x_coo.begin(), x_coo.end());
    //vector<int> y_temp(y_coo.begin(), y_coo.end());

    vector<int> *x_temp = new vector<int>;
    vector<int> *y_temp = new vector<int>;

    std:cout << !(1==1) << "\n";

    *x_temp = x_coo;
    *y_temp = y_coo;

    for (int i = 0; i < 4; i++)
    {
        std::cout << (*x_temp)[i] << " " << (*y_temp)[i] << "\n";
    }

    delete[] x_temp;
    delete[] y_temp;

    for (int i = 0; i < 4; i++)
    {
        std::cout << (*x_temp)[i] << " " << (*y_temp)[i] << "\n";
    }



}