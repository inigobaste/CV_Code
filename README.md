# The Game of Life

ACSE-6 OpenMP Group Assignment 2021

_Orkan Sezer, Iñigo Basterretxea Jacob, Nina Kahr_

## Prerequisites

When compiling, the `-std=c++17` and `-fopenmp` flags are required.
For MacOS, can build and run with `g++ -c -std=c++17 -fopenmp -O3 -march=native **/*.cpp && g++ -fopenmp **/*.o && ./a.out`.

## Classes

### Grid

A dense storage system for the game of life.

#### Properties

| Name      | Type                | Description                                     |
| --------- | ------------------- | ----------------------------------------------- |
| nrows     | `int`               | number of rows                                  |
| ncols     | `int`               | number of columns                               |
| cells     | `std::vector<bool>` | stores the state (dead or alive) of each cell   |
| new_cells | `std::vector<bool>` | temporary storage of updated states             |
| parallel  | `bool`              | whether to apply parallelisation in the methods |

#### Methods

- `num_neighbours`: returns the number of alive neighbours of a cell

- `to_file`: creates a `.dat` file of 0's and 1's representing the states of the cells

- `do_iteration`: compute next generation of the grid cell states, either in serial or parallel

- `do_iteration_serial`: compute next generation of the grid cell states in serial

- `do_iteration_parallel`: compute next generation of the grid cell states in parallel

- `time_data_to_file`: writes timings to .dat file

- `dense_to_COO`: shared pointer, converts `Grid` object to `COOGrid` object

### COOGrid

A sparse equivalent of `Grid` using coordinate list (COO) storage.

#### Properties

| Name       | Type                               | Description                                         |
| ---------- | ---------------------------------- | --------------------------------------------------- |
| nrows      | `int`                              | number of rows                                      |
| ncols      | `int`                              | number of columns                                   |
| coords     | `std::vector<std::pair<int, int>>` | vector of pairs of (x, y) coordinates of live cells |
| new_coords | `std::vector<std::pair<int, int>>` | temporary storage of updated coordinates            |
| parallel   | `bool`                             | whether to apply parallelisation in the methods     |

#### Methods

- `do_iteration_serial`: compute next generation of the grid cell states in serial

- `do_iteration_parallel`: compute next generation of the grid cell states in parallel

- `COO_to_dense`: shared pointer, converts `COOGrid` object to `Grid` object

## Testing

As a test framework, we are using [doctest](https://github.com/onqtam/doctest), chosen because it's lightweight and doesn't have a huge impact on compile or run times. The doctest header is included in `./doctest.h` and tests should be added in `./tests.cpp`.

To run this program without tests, add the `--no-run` flag when running the executable.

The tests are executed for the `Grid` method `do_iteration` and the `COOGrid` methods `do_iteration_serial` and `do_iteration_parallel`.

### Tests

1 - First Iteration for "Block" Creation: 5x6 grid forming a "block" after first iteration

2 - Test for 10x10 Grid: Elementwise accuracy check for an iteration over a 10x10 grid

3 - Steady State Check: Iterates twice over the first test's input to check if steady state is achieved (since the "block" does not change)

4 - Generation Counter Check: Tests accuracy of `generation` (for `Grid` method `do_iteration` only)

## Why COO Storage?

![Test Image 1](https://github.com/acse-2020/group-project-parallel-panicking/blob/main/Screen%20Shot%202021-02-17%20at%2018.27.36.png)
![Test Image 1](https://github.com/acse-2020/group-project-parallel-panicking/blob/main/Screen%20Shot%202021-02-17%20at%2018.27.48.png)

## Image printing

We chose to create our images in bitmap format since the bitmap structure is fairly straightforward and the grid data structure lends itself to writing to a .bmp file. The first part of the file is a header which contains metadata about how much data is stored and how to display the pixels. Then our cell data is written to the file from the last line to the first. Each row needs to be a multiple of 4 bytes long, so if our grid width doesn't fulfill that, we add some padding at the end. The code was inspired by this [StackOverflow question](https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries).

For more information see [this article](https://medium.com/sysf/bits-to-bitmaps-a-simple-walkthrough-of-bmp-image-format-765dc6857393).

## License

[MIT](https://choosealicense.com/licenses/mit/)
