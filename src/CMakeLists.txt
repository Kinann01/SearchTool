add_executable(SearchToolRunner main.cpp threadPool.cpp searcher.cpp)

option(ENABLE_DEBUG "Enable debugging" OFF)
option(ENABLE_READ_FROM_FILE "Enable reading input from file" OFF)
option(ENABLE_ONE_INSTANCE "Enable showing only one instance of output" OFF)

if (ENABLE_DEBUG)
    target_compile_definitions(SearchToolRunner PRIVATE DEBUG)
endif()

if (ENABLE_READ_FROM_FILE)
    target_compile_definitions(SearchToolRunner PRIVATE READ_FROM_FILE)
endif()

if (ENABLE_ONE_INSTANCE)
    target_compile_definitions(SearchToolRunner PRIVATE ONE_INSTANCE)
endif()
