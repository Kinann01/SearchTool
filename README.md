# File Search Tool 

## Introduction
- This repository contains a search tool implemented in C++. The tool allows users to find files containing a specified string provided by the user. It provides options to specify the search criteria, including the search starting point, the search string, specific file extensions to look into, and whether to include hidden files or not in the search. In short, its what the command `find . -name "*.EXT" | xargs grep "Search String"` in UNIX-like systems does.

## Dependencies

- The search tool requires the following dependencies:
    * CMake: Build system
    * Make: Build system
    * A compiler that supports at least c++20

## Usage 

```bash
# Build the Project:
mkdir build
cd build
cmake ..
make SearchToolRunner

# Run the Search Tool:
./src/SearchToolRunner
```

## Input Options
- The search tool provides options to input search criteria:
    * Path to start the search from.
    * String to search for in files.
    * File extensions to search for (leave empty for all files).
    * Whether to include search in hidden files (1 for yes, 0 for no).

## Additional Configuration (Optional)
- The search tool supports additional configuration through preprocessor directives:
    * `#DEBUG`: Enable debugging output.
    * `#READ_FROM_FILE`: Read input from a configuration file. Refer to `config.txt` for an example of the input configuration file. Once the project is run, the user will have to input the path to the configuration file.
    * `#ONE_INSTANCE`: Limit output to one instance of the search string.
- To enable the above options, modify the cmake command as follows:

```cmake
option(ENABLE_DEBUG "Enable debugging" ON)
option(ENABLE_READ_FROM_FILE "Enable reading input from file" ON)
option(ENABLE_ONE_INSTANCE "Enable showing only one instance of output" ON)
```

- Optionally, you can build the project with the following command: `cmake -DENABLE_DEBUG=ON or =OFF -DENABLE_READ_FROM_FILE=ON or =OFF -DENABLE_ONE_INSTANCE=ON or =OFF ..`, as prefered, instead of just `cmake ..`

## Implementation Details
- The search is conducted in parallel using the **Thread Pool** design pattern. The `thread_pool_t` class, implemented in `include/threadPool.h` and `src/threadPool.cpp`, manages a pool of threads that concurrently search for the specified string in files.

- The `thread_pool_t` class is designed to be thread safe allowing proper synchronization between threads. It uses a `std::condition_variable` to notify threads when tasks are available for execution, a `std::mutex` to protect shared resources, and a `std::queue` to store tasks.

- There are two constructors for the `thread_pool_t` type:
   * A parametrized constructor that allows specifying the number of threads in the pool.
   * A default constructor that automatically sets the pool size to the number of hardware threads available, as determined by `std::thread::hardware_concurrency()`.

- The `std::filesystem` library is utilized to traverse directories and search for files. Starting from the specified path, the search recursively enqueues tasks to the thread pool, searching through each directory and its subdirectories. Implementation details can be found in `src/searcher.cpp` and `include/searcher.h`.

- The `searcher_t` class is also designed to be thread safe. It uses a `std::mutex` to ensure proper access to output operations.

## Results
- The search tool outputs the paths of files containing the specified search string directly in the terminal. When in DEBUG mode, be careful not to miss them.

## Nuances
- There are some limitations to be aware of:
   * It is advisable to ensure that the files being searched contain the specified string, to prevent running unnecessary searches that yield no results.
   * It is recommended that all the search criteria is provided since proper exception handling is not implemented.