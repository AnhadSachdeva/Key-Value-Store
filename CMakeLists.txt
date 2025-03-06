cmake_minimum_required(VERSION 3.10)
project(my_redis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

include_directories(include)

# Main server executable
add_executable(my_redis_server
    src/main.cpp
    src/server.cpp
    src/storage_engine.cpp
    src/data_structures/hash_table.cpp
    src/data_structures/heap.cpp
    src/data_structures/sorted_set.cpp
    src/commands/command_handler.cpp
)

# Manual test executable
add_executable(manual_test
    manual_test.cpp
    src/storage_engine.cpp
    src/data_structures/hash_table.cpp
    src/data_structures/heap.cpp
    src/data_structures/sorted_set.cpp
)

# Install
install(TARGETS my_redis_server DESTINATION bin) 