# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.19.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.19.5/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/podor/Downloads/fswatch-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/podor/Downloads/fswatch-master/libfswatch

# Include any dependencies generated for this target.
include test/src/CMakeFiles/fswatch_test.dir/depend.make

# Include the progress variables for this target.
include test/src/CMakeFiles/fswatch_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/src/CMakeFiles/fswatch_test.dir/flags.make

test/src/CMakeFiles/fswatch_test.dir/fswatch_test.c.o: test/src/CMakeFiles/fswatch_test.dir/flags.make
test/src/CMakeFiles/fswatch_test.dir/fswatch_test.c.o: ../test/src/fswatch_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/src/CMakeFiles/fswatch_test.dir/fswatch_test.c.o"
	cd /Users/podor/Downloads/fswatch-master/libfswatch/test/src && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/fswatch_test.dir/fswatch_test.c.o -c /Users/podor/Downloads/fswatch-master/test/src/fswatch_test.c

test/src/CMakeFiles/fswatch_test.dir/fswatch_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fswatch_test.dir/fswatch_test.c.i"
	cd /Users/podor/Downloads/fswatch-master/libfswatch/test/src && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/podor/Downloads/fswatch-master/test/src/fswatch_test.c > CMakeFiles/fswatch_test.dir/fswatch_test.c.i

test/src/CMakeFiles/fswatch_test.dir/fswatch_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fswatch_test.dir/fswatch_test.c.s"
	cd /Users/podor/Downloads/fswatch-master/libfswatch/test/src && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/podor/Downloads/fswatch-master/test/src/fswatch_test.c -o CMakeFiles/fswatch_test.dir/fswatch_test.c.s

# Object files for target fswatch_test
fswatch_test_OBJECTS = \
"CMakeFiles/fswatch_test.dir/fswatch_test.c.o"

# External object files for target fswatch_test
fswatch_test_EXTERNAL_OBJECTS =

test/src/fswatch_test: test/src/CMakeFiles/fswatch_test.dir/fswatch_test.c.o
test/src/fswatch_test: test/src/CMakeFiles/fswatch_test.dir/build.make
test/src/fswatch_test: libfswatch/libfswatch.a
test/src/fswatch_test: /usr/local/lib/libintl.dylib
test/src/fswatch_test: /Library/Developer/CommandLineTools/SDKs/MacOSX11.3.sdk/usr/lib/libpthread.tbd
test/src/fswatch_test: test/src/CMakeFiles/fswatch_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fswatch_test"
	cd /Users/podor/Downloads/fswatch-master/libfswatch/test/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fswatch_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/src/CMakeFiles/fswatch_test.dir/build: test/src/fswatch_test

.PHONY : test/src/CMakeFiles/fswatch_test.dir/build

test/src/CMakeFiles/fswatch_test.dir/clean:
	cd /Users/podor/Downloads/fswatch-master/libfswatch/test/src && $(CMAKE_COMMAND) -P CMakeFiles/fswatch_test.dir/cmake_clean.cmake
.PHONY : test/src/CMakeFiles/fswatch_test.dir/clean

test/src/CMakeFiles/fswatch_test.dir/depend:
	cd /Users/podor/Downloads/fswatch-master/libfswatch && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/podor/Downloads/fswatch-master /Users/podor/Downloads/fswatch-master/test/src /Users/podor/Downloads/fswatch-master/libfswatch /Users/podor/Downloads/fswatch-master/libfswatch/test/src /Users/podor/Downloads/fswatch-master/libfswatch/test/src/CMakeFiles/fswatch_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/src/CMakeFiles/fswatch_test.dir/depend

