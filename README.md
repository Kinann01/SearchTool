# File Search Tool 

## Introduction

- This repository contains a search tool implemented in C++. The tool allows users to search for a specific string within files located in a given directory and its subdirectories. It provides options to specify the search criteria, including the search string, file extensions to search for, and whether to include hidden files in the search. The output is a path to files containing the search string.

## Dependencies

- The search tool requires the following dependencies:
    * CMake: Build system
    * Make: Build system
    * A compiler that supports at least c++17

## Usage 

```bash
# Build the Project:
mkdir build
cd build
cmake ..
make

# Run the Search Tool:
./SearchToolRunner
```

## Input Options
- The search tool provides options to input search criteria:
 * Path to start the search from.
 * String to search for in files.
 * File extensions to search for (leave empty for all files).
 * Whether to include search in hidden files (1 for yes, 0 for no).


## Additional Configuration (Optional)
- The search tool supports additional configuration through preprocessor directives:
 * DEBUG: Enable debugging output.
 * READ_FROM_FILE: Read input from a configuration file.
 * ONE_INSTANCE: Limit output to one instance of the search string.
 * To enable these options, modify the cmake command as follows:

```cmake
option(ENABLE_DEBUG "Enable debugging" ON)
option(ENABLE_READ_FROM_FILE "Enable reading input from file" ON)
option(ENABLE_ONE_INSTANCE "Enable showing only one instance of output" ON)
```

- Optionally you can just build the project with the following command: `cmake -DENABLE_DEBUG=ON -DENABLE_READ_FROM_FILE=ON -DENABLE_ONE_INSTANCE=ON ..` instead of just `cmake ..`

## Tools used 
- The search takes place in parallel using the Thread Pool design pattern. The tool uses the thread_pool_t class to create a pool of threads that search for the string in files concurrently, it is implemented in `include/threadPool.h` and `src/threadPool.cpp` files. There are two constructors provided for the `thread_pool_t` type, a parametrized constructor that takes the number of threads to create in the pool and a default constructor that creates a pool with the number of threads equal to the number of hardware threads available on the system, this is supported using, this is supported using `std::thread::hardware_concurrency()`. The `enqueue` method is used to add tasks to the thread pool. The destructor ensures that all threads are joined.

- The search utilizes the `std::filesystem` library to traverse directories and search for files. The search starts with the provided path and recursively searches for files in the directory and its subdirectories. Refer to `src/searcher.cpp` and `include/searcher.h` for the implementation of the search functionality.

## Results
- The search tool will output in the terminal the paths of files containing the search string

## Nuances
- There can be some nuances in the search tool, for example, the search tool may not be able to search for files in directories that the user does not have permission to access. In such cases, the search tool will output an error message indicating that the directory could not be accessed. 

- It is also recommended that the user is sure a file with the specified string exists in the file system before running the search tool.