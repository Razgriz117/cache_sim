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
include CMakeFiles/mem_architecture_sim.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mem_architecture_sim.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mem_architecture_sim.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mem_architecture_sim.dir/flags.make

CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o: CMakeFiles/mem_architecture_sim.dir/flags.make
CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o: ../mem_architecture_sim.cpp
CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o: CMakeFiles/mem_architecture_sim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o -MF CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o.d -o CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o -c /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_architecture_sim.cpp

CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_architecture_sim.cpp > CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.i

CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/mem_architecture_sim.cpp -o CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.s

CMakeFiles/mem_architecture_sim.dir/main.cpp.o: CMakeFiles/mem_architecture_sim.dir/flags.make
CMakeFiles/mem_architecture_sim.dir/main.cpp.o: ../main.cpp
CMakeFiles/mem_architecture_sim.dir/main.cpp.o: CMakeFiles/mem_architecture_sim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/mem_architecture_sim.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mem_architecture_sim.dir/main.cpp.o -MF CMakeFiles/mem_architecture_sim.dir/main.cpp.o.d -o CMakeFiles/mem_architecture_sim.dir/main.cpp.o -c /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/main.cpp

CMakeFiles/mem_architecture_sim.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mem_architecture_sim.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/main.cpp > CMakeFiles/mem_architecture_sim.dir/main.cpp.i

CMakeFiles/mem_architecture_sim.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mem_architecture_sim.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/main.cpp -o CMakeFiles/mem_architecture_sim.dir/main.cpp.s

# Object files for target mem_architecture_sim
mem_architecture_sim_OBJECTS = \
"CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o" \
"CMakeFiles/mem_architecture_sim.dir/main.cpp.o"

# External object files for target mem_architecture_sim
mem_architecture_sim_EXTERNAL_OBJECTS =

mem_architecture_sim: CMakeFiles/mem_architecture_sim.dir/mem_architecture_sim.cpp.o
mem_architecture_sim: CMakeFiles/mem_architecture_sim.dir/main.cpp.o
mem_architecture_sim: CMakeFiles/mem_architecture_sim.dir/build.make
mem_architecture_sim: mem_cache/libmem_cache.a
mem_architecture_sim: CMakeFiles/mem_architecture_sim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable mem_architecture_sim"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mem_architecture_sim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mem_architecture_sim.dir/build: mem_architecture_sim
.PHONY : CMakeFiles/mem_architecture_sim.dir/build

CMakeFiles/mem_architecture_sim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mem_architecture_sim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mem_architecture_sim.dir/clean

CMakeFiles/mem_architecture_sim.dir/depend:
	cd /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build /home/razgriz117/Martin/CodeDirectory/UCF/architecture/program1/simulator/build/CMakeFiles/mem_architecture_sim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mem_architecture_sim.dir/depend
