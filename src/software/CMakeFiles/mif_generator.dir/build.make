# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_SOURCE_DIR = /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software

# Include any dependencies generated for this target.
include CMakeFiles/mif_generator.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mif_generator.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mif_generator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mif_generator.dir/flags.make

CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o: CMakeFiles/mif_generator.dir/flags.make
CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o: src/mif_generator.cpp
CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o: CMakeFiles/mif_generator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mjc284/Documents/Work/mjc284_fsm_processor/src/software/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o -MF CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o.d -o CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o -c /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software/src/mif_generator.cpp

CMakeFiles/mif_generator.dir/src/mif_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mif_generator.dir/src/mif_generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software/src/mif_generator.cpp > CMakeFiles/mif_generator.dir/src/mif_generator.cpp.i

CMakeFiles/mif_generator.dir/src/mif_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mif_generator.dir/src/mif_generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software/src/mif_generator.cpp -o CMakeFiles/mif_generator.dir/src/mif_generator.cpp.s

# Object files for target mif_generator
mif_generator_OBJECTS = \
"CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o"

# External object files for target mif_generator
mif_generator_EXTERNAL_OBJECTS =

build/mif_generator: CMakeFiles/mif_generator.dir/src/mif_generator.cpp.o
build/mif_generator: CMakeFiles/mif_generator.dir/build.make
build/mif_generator: CMakeFiles/mif_generator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mjc284/Documents/Work/mjc284_fsm_processor/src/software/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable build/mif_generator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mif_generator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mif_generator.dir/build: build/mif_generator
.PHONY : CMakeFiles/mif_generator.dir/build

CMakeFiles/mif_generator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mif_generator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mif_generator.dir/clean

CMakeFiles/mif_generator.dir/depend:
	cd /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software /home/mjc284/Documents/Work/mjc284_fsm_processor/src/software/CMakeFiles/mif_generator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mif_generator.dir/depend

