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

# Utility rule file for potfiles_1.

# Include the progress variables for this target.
include CMakeFiles/potfiles_1.dir/progress.make

CMakeFiles/potfiles_1: en.gmo
CMakeFiles/potfiles_1: en@quot.gmo
CMakeFiles/potfiles_1: en@boldquot.gmo
CMakeFiles/potfiles_1: it.gmo
CMakeFiles/potfiles_1: es.gmo


en.gmo: ../po/fswatch.pot
en.gmo: en.po
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating en.gmo"
	/usr/local/bin/msgfmt -o /Users/podor/Downloads/fswatch-master/libfswatch/en.gmo /Users/podor/Downloads/fswatch-master/libfswatch/en.po

en@quot.gmo: ../po/fswatch.pot
en@quot.gmo: en@quot.po
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating en@quot.gmo"
	/usr/local/bin/msgfmt -o /Users/podor/Downloads/fswatch-master/libfswatch/en@quot.gmo /Users/podor/Downloads/fswatch-master/libfswatch/en@quot.po

en@boldquot.gmo: ../po/fswatch.pot
en@boldquot.gmo: en@boldquot.po
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating en@boldquot.gmo"
	/usr/local/bin/msgfmt -o /Users/podor/Downloads/fswatch-master/libfswatch/en@boldquot.gmo /Users/podor/Downloads/fswatch-master/libfswatch/en@boldquot.po

it.gmo: ../po/fswatch.pot
it.gmo: it.po
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating it.gmo"
	/usr/local/bin/msgfmt -o /Users/podor/Downloads/fswatch-master/libfswatch/it.gmo /Users/podor/Downloads/fswatch-master/libfswatch/it.po

es.gmo: ../po/fswatch.pot
es.gmo: es.po
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Generating es.gmo"
	/usr/local/bin/msgfmt -o /Users/podor/Downloads/fswatch-master/libfswatch/es.gmo /Users/podor/Downloads/fswatch-master/libfswatch/es.po

en.po: ../po/fswatch.pot
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Generating en.po"
	/usr/local/bin/msgmerge --quiet --update --backup=none -s /Users/podor/Downloads/fswatch-master/libfswatch/en.po /Users/podor/Downloads/fswatch-master/po/fswatch.pot

en@quot.po: ../po/fswatch.pot
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Generating en@quot.po"
	/usr/local/bin/msgmerge --quiet --update --backup=none -s /Users/podor/Downloads/fswatch-master/libfswatch/en@quot.po /Users/podor/Downloads/fswatch-master/po/fswatch.pot

en@boldquot.po: ../po/fswatch.pot
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Generating en@boldquot.po"
	/usr/local/bin/msgmerge --quiet --update --backup=none -s /Users/podor/Downloads/fswatch-master/libfswatch/en@boldquot.po /Users/podor/Downloads/fswatch-master/po/fswatch.pot

it.po: ../po/fswatch.pot
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Generating it.po"
	/usr/local/bin/msgmerge --quiet --update --backup=none -s /Users/podor/Downloads/fswatch-master/libfswatch/it.po /Users/podor/Downloads/fswatch-master/po/fswatch.pot

es.po: ../po/fswatch.pot
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Generating es.po"
	/usr/local/bin/msgmerge --quiet --update --backup=none -s /Users/podor/Downloads/fswatch-master/libfswatch/es.po /Users/podor/Downloads/fswatch-master/po/fswatch.pot

potfiles_1: CMakeFiles/potfiles_1
potfiles_1: en.gmo
potfiles_1: en.po
potfiles_1: en@boldquot.gmo
potfiles_1: en@boldquot.po
potfiles_1: en@quot.gmo
potfiles_1: en@quot.po
potfiles_1: es.gmo
potfiles_1: es.po
potfiles_1: it.gmo
potfiles_1: it.po
potfiles_1: CMakeFiles/potfiles_1.dir/build.make

.PHONY : potfiles_1

# Rule to build all files generated by this target.
CMakeFiles/potfiles_1.dir/build: potfiles_1

.PHONY : CMakeFiles/potfiles_1.dir/build

CMakeFiles/potfiles_1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/potfiles_1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/potfiles_1.dir/clean

CMakeFiles/potfiles_1.dir/depend:
	cd /Users/podor/Downloads/fswatch-master/libfswatch && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/podor/Downloads/fswatch-master /Users/podor/Downloads/fswatch-master /Users/podor/Downloads/fswatch-master/libfswatch /Users/podor/Downloads/fswatch-master/libfswatch /Users/podor/Downloads/fswatch-master/libfswatch/CMakeFiles/potfiles_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/potfiles_1.dir/depend
