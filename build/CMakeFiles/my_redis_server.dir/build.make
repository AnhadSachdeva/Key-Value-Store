# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lappy/Desktop/projects/my-redis

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lappy/Desktop/projects/my-redis/build

# Include any dependencies generated for this target.
include CMakeFiles/my_redis_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/my_redis_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/my_redis_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/my_redis_server.dir/flags.make

CMakeFiles/my_redis_server.dir/codegen:
.PHONY : CMakeFiles/my_redis_server.dir/codegen

CMakeFiles/my_redis_server.dir/src/main.cpp.o: CMakeFiles/my_redis_server.dir/flags.make
CMakeFiles/my_redis_server.dir/src/main.cpp.o: /Users/lappy/Desktop/projects/my-redis/src/main.cpp
CMakeFiles/my_redis_server.dir/src/main.cpp.o: CMakeFiles/my_redis_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lappy/Desktop/projects/my-redis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/my_redis_server.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/my_redis_server.dir/src/main.cpp.o -MF CMakeFiles/my_redis_server.dir/src/main.cpp.o.d -o CMakeFiles/my_redis_server.dir/src/main.cpp.o -c /Users/lappy/Desktop/projects/my-redis/src/main.cpp

CMakeFiles/my_redis_server.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/my_redis_server.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lappy/Desktop/projects/my-redis/src/main.cpp > CMakeFiles/my_redis_server.dir/src/main.cpp.i

CMakeFiles/my_redis_server.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/my_redis_server.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lappy/Desktop/projects/my-redis/src/main.cpp -o CMakeFiles/my_redis_server.dir/src/main.cpp.s

CMakeFiles/my_redis_server.dir/src/server.cpp.o: CMakeFiles/my_redis_server.dir/flags.make
CMakeFiles/my_redis_server.dir/src/server.cpp.o: /Users/lappy/Desktop/projects/my-redis/src/server.cpp
CMakeFiles/my_redis_server.dir/src/server.cpp.o: CMakeFiles/my_redis_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lappy/Desktop/projects/my-redis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/my_redis_server.dir/src/server.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/my_redis_server.dir/src/server.cpp.o -MF CMakeFiles/my_redis_server.dir/src/server.cpp.o.d -o CMakeFiles/my_redis_server.dir/src/server.cpp.o -c /Users/lappy/Desktop/projects/my-redis/src/server.cpp

CMakeFiles/my_redis_server.dir/src/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/my_redis_server.dir/src/server.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lappy/Desktop/projects/my-redis/src/server.cpp > CMakeFiles/my_redis_server.dir/src/server.cpp.i

CMakeFiles/my_redis_server.dir/src/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/my_redis_server.dir/src/server.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lappy/Desktop/projects/my-redis/src/server.cpp -o CMakeFiles/my_redis_server.dir/src/server.cpp.s

CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o: CMakeFiles/my_redis_server.dir/flags.make
CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o: /Users/lappy/Desktop/projects/my-redis/src/storage_engine.cpp
CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o: CMakeFiles/my_redis_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lappy/Desktop/projects/my-redis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o -MF CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o.d -o CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o -c /Users/lappy/Desktop/projects/my-redis/src/storage_engine.cpp

CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lappy/Desktop/projects/my-redis/src/storage_engine.cpp > CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.i

CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lappy/Desktop/projects/my-redis/src/storage_engine.cpp -o CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.s

CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o: CMakeFiles/my_redis_server.dir/flags.make
CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o: /Users/lappy/Desktop/projects/my-redis/src/data_structures/hash_table.cpp
CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o: CMakeFiles/my_redis_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lappy/Desktop/projects/my-redis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o -MF CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o.d -o CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o -c /Users/lappy/Desktop/projects/my-redis/src/data_structures/hash_table.cpp

CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lappy/Desktop/projects/my-redis/src/data_structures/hash_table.cpp > CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.i

CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lappy/Desktop/projects/my-redis/src/data_structures/hash_table.cpp -o CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.s

CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o: CMakeFiles/my_redis_server.dir/flags.make
CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o: /Users/lappy/Desktop/projects/my-redis/src/data_structures/heap.cpp
CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o: CMakeFiles/my_redis_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lappy/Desktop/projects/my-redis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o -MF CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o.d -o CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o -c /Users/lappy/Desktop/projects/my-redis/src/data_structures/heap.cpp

CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lappy/Desktop/projects/my-redis/src/data_structures/heap.cpp > CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.i

CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lappy/Desktop/projects/my-redis/src/data_structures/heap.cpp -o CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.s

CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o: CMakeFiles/my_redis_server.dir/flags.make
CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o: /Users/lappy/Desktop/projects/my-redis/src/data_structures/sorted_set.cpp
CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o: CMakeFiles/my_redis_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lappy/Desktop/projects/my-redis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o -MF CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o.d -o CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o -c /Users/lappy/Desktop/projects/my-redis/src/data_structures/sorted_set.cpp

CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lappy/Desktop/projects/my-redis/src/data_structures/sorted_set.cpp > CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.i

CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lappy/Desktop/projects/my-redis/src/data_structures/sorted_set.cpp -o CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.s

CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o: CMakeFiles/my_redis_server.dir/flags.make
CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o: /Users/lappy/Desktop/projects/my-redis/src/commands/command_handler.cpp
CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o: CMakeFiles/my_redis_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/lappy/Desktop/projects/my-redis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o -MF CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o.d -o CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o -c /Users/lappy/Desktop/projects/my-redis/src/commands/command_handler.cpp

CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lappy/Desktop/projects/my-redis/src/commands/command_handler.cpp > CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.i

CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lappy/Desktop/projects/my-redis/src/commands/command_handler.cpp -o CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.s

# Object files for target my_redis_server
my_redis_server_OBJECTS = \
"CMakeFiles/my_redis_server.dir/src/main.cpp.o" \
"CMakeFiles/my_redis_server.dir/src/server.cpp.o" \
"CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o" \
"CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o" \
"CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o" \
"CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o" \
"CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o"

# External object files for target my_redis_server
my_redis_server_EXTERNAL_OBJECTS =

my_redis_server: CMakeFiles/my_redis_server.dir/src/main.cpp.o
my_redis_server: CMakeFiles/my_redis_server.dir/src/server.cpp.o
my_redis_server: CMakeFiles/my_redis_server.dir/src/storage_engine.cpp.o
my_redis_server: CMakeFiles/my_redis_server.dir/src/data_structures/hash_table.cpp.o
my_redis_server: CMakeFiles/my_redis_server.dir/src/data_structures/heap.cpp.o
my_redis_server: CMakeFiles/my_redis_server.dir/src/data_structures/sorted_set.cpp.o
my_redis_server: CMakeFiles/my_redis_server.dir/src/commands/command_handler.cpp.o
my_redis_server: CMakeFiles/my_redis_server.dir/build.make
my_redis_server: CMakeFiles/my_redis_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/lappy/Desktop/projects/my-redis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable my_redis_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/my_redis_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/my_redis_server.dir/build: my_redis_server
.PHONY : CMakeFiles/my_redis_server.dir/build

CMakeFiles/my_redis_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/my_redis_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/my_redis_server.dir/clean

CMakeFiles/my_redis_server.dir/depend:
	cd /Users/lappy/Desktop/projects/my-redis/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lappy/Desktop/projects/my-redis /Users/lappy/Desktop/projects/my-redis /Users/lappy/Desktop/projects/my-redis/build /Users/lappy/Desktop/projects/my-redis/build /Users/lappy/Desktop/projects/my-redis/build/CMakeFiles/my_redis_server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/my_redis_server.dir/depend

