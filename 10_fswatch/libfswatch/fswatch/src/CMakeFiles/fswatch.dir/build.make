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
include fswatch/src/CMakeFiles/fswatch.dir/depend.make

# Include the progress variables for this target.
include fswatch/src/CMakeFiles/fswatch.dir/progress.make

# Include the compile flags for this target's objects.
include fswatch/src/CMakeFiles/fswatch.dir/flags.make

fswatch/src/CMakeFiles/fswatch.dir/fswatch.cpp.o: fswatch/src/CMakeFiles/fswatch.dir/flags.make
fswatch/src/CMakeFiles/fswatch.dir/fswatch.cpp.o: ../fswatch/src/fswatch.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object fswatch/src/CMakeFiles/fswatch.dir/fswatch.cpp.o"
	cd /Users/podor/Downloads/fswatch-master/libfswatch/fswatch/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fswatch.dir/fswatch.cpp.o -c /Users/podor/Downloads/fswatch-master/fswatch/src/fswatch.cpp

fswatch/src/CMakeFiles/fswatch.dir/fswatch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fswatch.dir/fswatch.cpp.i"
	cd /Users/podor/Downloads/fswatch-master/libfswatch/fswatch/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/podor/Downloads/fswatch-master/fswatch/src/fswatch.cpp > CMakeFiles/fswatch.dir/fswatch.cpp.i

fswatch/src/CMakeFiles/fswatch.dir/fswatch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fswatch.dir/fswatch.cpp.s"
	cd /Users/podor/Downloads/fswatch-master/libfswatch/fswatch/src && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/podor/Downloads/fswatch-master/fswatch/src/fswatch.cpp -o CMakeFiles/fswatch.dir/fswatch.cpp.s

# Object files for target fswatch
fswatch_OBJECTS = \
"CMakeFiles/fswatch.dir/fswatch.cpp.o"

# External object files for target fswatch
fswatch_EXTERNAL_OBJECTS =

fswatch/src/fswatch: fswatch/src/CMakeFiles/fswatch.dir/fswatch.cpp.o
fswatch/src/fswatch: fswatch/src/CMakeFiles/fswatch.dir/build.make
fswatch/src/fswatch: /usr/local/lib/libintl.dylib
fswatch/src/fswatch: libfswatch/libfswatch.a
fswatch/src/fswatch: /usr/local/lib/libintl.dylib
fswatch/src/fswatch: /Library/Developer/CommandLineTools/SDKs/MacOSX11.3.sdk/usr/lib/libpthread.tbd
fswatch/src/fswatch: fswatch/src/CMakeFiles/fswatch.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fswatch"
	cd /Users/podor/Downloads/fswatch-master/libfswatch/fswatch/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fswatch.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
fswatch/src/CMakeFiles/fswatch.dir/build: fswatch/src/fswatch

.PHONY : fswatch/src/CMakeFiles/fswatch.dir/build

fswatch/src/CMakeFiles/fswatch.dir/clean:
	cd /Users/podor/Downloads/fswatch-master/libfswatch/fswatch/src && $(CMAKE_COMMAND) -P CMakeFiles/fswatch.dir/cmake_clean.cmake
.PHONY : fswatch/src/CMakeFiles/fswatch.dir/clean

fswatch/src/CMakeFiles/fswatch.dir/depend:
	cd /Users/podor/Downloads/fswatch-master/libfswatch && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/podor/Downloads/fswatch-master /Users/podor/Downloads/fswatch-master/fswatch/src /Users/podor/Downloads/fswatch-master/libfswatch /Users/podor/Downloads/fswatch-master/libfswatch/fswatch/src /Users/podor/Downloads/fswatch-master/libfswatch/fswatch/src/CMakeFiles/fswatch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : fswatch/src/CMakeFiles/fswatch.dir/depend

