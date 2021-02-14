# The Game of Life

ACSE-6 OpenMP Group Assignment 2021

_Orkan Sezer, Iñigo Basterretxea Jacob, Nina Kahr_

## Prerequisites

When compiling, the `-std=c++17` and `-fopenmp` flags are required.

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

`num_neighbours`: returns the number of alive neighbours of a cell
`to_file`: creates a `.dat` file of 0's and 1's representing the states of the cells
`do_iteration`: compute next generation of the game state

### COOGrid

A sparse equivalent of `Grid`.

#### Properties

| Name       | Type                               | Description                                         |
| ---------- | ---------------------------------- | --------------------------------------------------- |
| nrows      | `int`                              | number of rows                                      |
| ncols      | `int`                              | number of columns                                   |
| coords     | `std::vector<std::pair<int, int>>` | vector of pairs of (x, y) coordinates of live cells |
| new_coords | `std::vector<std::pair<int, int>>` | temporary storage of updated coordinates            |
| parallel   | `bool`                             | whether to apply parallelisation in the methods     |

## License

[MIT](https://choosealicense.com/licenses/mit/)
