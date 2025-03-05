# Custom Redis-Inspired Key-Value Store

Inspired by the book "Build your own Redis in C/C++" by James Smith. The book implements in C basically which goes pretty low level.

A custom in-memory key-value store inspired by Redis, implemented in C++. This project includes custom implementations of various data structures and provides a Redis-like interface for storing and retrieving data.




## Features

- In-memory key-value storage
- TTL (Time-To-Live) expiration
- Non-blocking I/O using epoll
- Socket-based communication
- Multithreaded design for concurrent operations

### Supported Commands

- **Basic Commands**: SET, GET, DEL, EXISTS, EXPIRE, TTL, DBSIZE, FLUSHDB
- **Sorted Set Commands**: ZADD, ZREM, ZSCORE, ZRANGE, ZRANGEBYSCORE, ZRANK, ZCARD

### Custom Data Structures

- **Hash Table**: For primary key-value storage
- **AVL Tree**: For efficient sorted data structures
- **Heap**: For TTL management
- **Sorted Set**: Implemented using AVL tree and hash map

## Building the Project


### Build Instructions

```bash
# Create a build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
make

# Run the server
./my_redis_server [port]
```

## Running Tests

The project includes a comprehensive Python test script that tests all functionality:

```bash
# Navigate to the tests directory
cd tests

# Run the tests
python3 test_redis.py
```

## Implementation Details

### Architecture

The project is organized into several components:

1. **Data Structures**: Custom implementations of hash table, AVL tree, heap, and sorted set
2. **Storage Engine**: Core storage functionality with TTL management
3. **Command Handler**: Processes Redis-like commands
4. **Server**: Handles client connections and network I/O

### Performance Considerations

- Non-blocking I/O for handling multiple connections
- Efficient data structures for different operations
- Thread-safe implementations for concurrent access
- Optimized memory usage

## Future Improvements

- More Redis commands (Lists, Sets, Hashes)


## Acknowledgements

This project was inspired by Redis, the popular in-memory data structure store. 

"Build your own Redis in C/C++" by James Smith
