# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build

# Include any dependencies generated for this target.
include mem_cache/CMakeFiles/mem_cache.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include mem_cache/CMakeFiles/mem_cache.dir/compiler_depend.make

# Include the progress variables for this target.
include mem_cache/CMakeFiles/mem_cache.dir/progress.make

# Include the compile flags for this target's objects.
include mem_cache/CMakeFiles/mem_cache.dir/flags.make

mem_cache/CMakeFiles/mem_cache.dir/src/address.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/flags.make
mem_cache/CMakeFiles/mem_cache.dir/src/address.cpp.o: ../mem_cache/src/address.cpp
mem_cache/CMakeFiles/mem_cache.dir/src/address.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mem_cache/CMakeFiles/mem_cache.dir/src/address.cpp.o"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT mem_cache/CMakeFiles/mem_cache.dir/src/address.cpp.o -MF CMakeFiles/mem_cache.dir/src/address.cpp.o.d -o CMakeFiles/mem_cache.dir/src/address.cpp.o -c /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/address.cpp

mem_cache/CMakeFiles/mem_cache.dir/src/address.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mem_cache.dir/src/address.cpp.i"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/address.cpp > CMakeFiles/mem_cache.dir/src/address.cpp.i

mem_cache/CMakeFiles/mem_cache.dir/src/address.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mem_cache.dir/src/address.cpp.s"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/address.cpp -o CMakeFiles/mem_cache.dir/src/address.cpp.s

mem_cache/CMakeFiles/mem_cache.dir/src/instruction.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/flags.make
mem_cache/CMakeFiles/mem_cache.dir/src/instruction.cpp.o: ../mem_cache/src/instruction.cpp
mem_cache/CMakeFiles/mem_cache.dir/src/instruction.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object mem_cache/CMakeFiles/mem_cache.dir/src/instruction.cpp.o"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT mem_cache/CMakeFiles/mem_cache.dir/src/instruction.cpp.o -MF CMakeFiles/mem_cache.dir/src/instruction.cpp.o.d -o CMakeFiles/mem_cache.dir/src/instruction.cpp.o -c /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/instruction.cpp

mem_cache/CMakeFiles/mem_cache.dir/src/instruction.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mem_cache.dir/src/instruction.cpp.i"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/instruction.cpp > CMakeFiles/mem_cache.dir/src/instruction.cpp.i

mem_cache/CMakeFiles/mem_cache.dir/src/instruction.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mem_cache.dir/src/instruction.cpp.s"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/instruction.cpp -o CMakeFiles/mem_cache.dir/src/instruction.cpp.s

mem_cache/CMakeFiles/mem_cache.dir/src/block.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/flags.make
mem_cache/CMakeFiles/mem_cache.dir/src/block.cpp.o: ../mem_cache/src/block.cpp
mem_cache/CMakeFiles/mem_cache.dir/src/block.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object mem_cache/CMakeFiles/mem_cache.dir/src/block.cpp.o"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT mem_cache/CMakeFiles/mem_cache.dir/src/block.cpp.o -MF CMakeFiles/mem_cache.dir/src/block.cpp.o.d -o CMakeFiles/mem_cache.dir/src/block.cpp.o -c /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/block.cpp

mem_cache/CMakeFiles/mem_cache.dir/src/block.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mem_cache.dir/src/block.cpp.i"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/block.cpp > CMakeFiles/mem_cache.dir/src/block.cpp.i

mem_cache/CMakeFiles/mem_cache.dir/src/block.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mem_cache.dir/src/block.cpp.s"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/block.cpp -o CMakeFiles/mem_cache.dir/src/block.cpp.s

mem_cache/CMakeFiles/mem_cache.dir/src/cache.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/flags.make
mem_cache/CMakeFiles/mem_cache.dir/src/cache.cpp.o: ../mem_cache/src/cache.cpp
mem_cache/CMakeFiles/mem_cache.dir/src/cache.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object mem_cache/CMakeFiles/mem_cache.dir/src/cache.cpp.o"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT mem_cache/CMakeFiles/mem_cache.dir/src/cache.cpp.o -MF CMakeFiles/mem_cache.dir/src/cache.cpp.o.d -o CMakeFiles/mem_cache.dir/src/cache.cpp.o -c /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/cache.cpp

mem_cache/CMakeFiles/mem_cache.dir/src/cache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mem_cache.dir/src/cache.cpp.i"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/cache.cpp > CMakeFiles/mem_cache.dir/src/cache.cpp.i

mem_cache/CMakeFiles/mem_cache.dir/src/cache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mem_cache.dir/src/cache.cpp.s"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/cache.cpp -o CMakeFiles/mem_cache.dir/src/cache.cpp.s

mem_cache/CMakeFiles/mem_cache.dir/src/set.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/flags.make
mem_cache/CMakeFiles/mem_cache.dir/src/set.cpp.o: ../mem_cache/src/set.cpp
mem_cache/CMakeFiles/mem_cache.dir/src/set.cpp.o: mem_cache/CMakeFiles/mem_cache.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object mem_cache/CMakeFiles/mem_cache.dir/src/set.cpp.o"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT mem_cache/CMakeFiles/mem_cache.dir/src/set.cpp.o -MF CMakeFiles/mem_cache.dir/src/set.cpp.o.d -o CMakeFiles/mem_cache.dir/src/set.cpp.o -c /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/set.cpp

mem_cache/CMakeFiles/mem_cache.dir/src/set.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mem_cache.dir/src/set.cpp.i"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/set.cpp > CMakeFiles/mem_cache.dir/src/set.cpp.i

mem_cache/CMakeFiles/mem_cache.dir/src/set.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mem_cache.dir/src/set.cpp.s"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache/src/set.cpp -o CMakeFiles/mem_cache.dir/src/set.cpp.s

# Object files for target mem_cache
mem_cache_OBJECTS = \
"CMakeFiles/mem_cache.dir/src/address.cpp.o" \
"CMakeFiles/mem_cache.dir/src/instruction.cpp.o" \
"CMakeFiles/mem_cache.dir/src/block.cpp.o" \
"CMakeFiles/mem_cache.dir/src/cache.cpp.o" \
"CMakeFiles/mem_cache.dir/src/set.cpp.o"

# External object files for target mem_cache
mem_cache_EXTERNAL_OBJECTS =

mem_cache/libmem_cache.a: mem_cache/CMakeFiles/mem_cache.dir/src/address.cpp.o
mem_cache/libmem_cache.a: mem_cache/CMakeFiles/mem_cache.dir/src/instruction.cpp.o
mem_cache/libmem_cache.a: mem_cache/CMakeFiles/mem_cache.dir/src/block.cpp.o
mem_cache/libmem_cache.a: mem_cache/CMakeFiles/mem_cache.dir/src/cache.cpp.o
mem_cache/libmem_cache.a: mem_cache/CMakeFiles/mem_cache.dir/src/set.cpp.o
mem_cache/libmem_cache.a: mem_cache/CMakeFiles/mem_cache.dir/build.make
mem_cache/libmem_cache.a: mem_cache/CMakeFiles/mem_cache.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libmem_cache.a"
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && $(CMAKE_COMMAND) -P CMakeFiles/mem_cache.dir/cmake_clean_target.cmake
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mem_cache.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mem_cache/CMakeFiles/mem_cache.dir/build: mem_cache/libmem_cache.a
.PHONY : mem_cache/CMakeFiles/mem_cache.dir/build

mem_cache/CMakeFiles/mem_cache.dir/clean:
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache && $(CMAKE_COMMAND) -P CMakeFiles/mem_cache.dir/cmake_clean.cmake
.PHONY : mem_cache/CMakeFiles/mem_cache.dir/clean

mem_cache/CMakeFiles/mem_cache.dir/depend:
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_cache /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/mem_cache/CMakeFiles/mem_cache.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mem_cache/CMakeFiles/mem_cache.dir/depend

