# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/willchambers/Projects/astral/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/willchambers/Projects/astral/src/renderer

# Utility rule file for NightlySubmit.

# Include the progress variables for this target.
include renderer/CMakeFiles/NightlySubmit.dir/progress.make

renderer/CMakeFiles/NightlySubmit:
	cd /Users/willchambers/Projects/astral/src/renderer/renderer && /usr/local/Cellar/cmake/3.15.5/bin/ctest -D NightlySubmit

NightlySubmit: renderer/CMakeFiles/NightlySubmit
NightlySubmit: renderer/CMakeFiles/NightlySubmit.dir/build.make

.PHONY : NightlySubmit

# Rule to build all files generated by this target.
renderer/CMakeFiles/NightlySubmit.dir/build: NightlySubmit

.PHONY : renderer/CMakeFiles/NightlySubmit.dir/build

renderer/CMakeFiles/NightlySubmit.dir/clean:
	cd /Users/willchambers/Projects/astral/src/renderer/renderer && $(CMAKE_COMMAND) -P CMakeFiles/NightlySubmit.dir/cmake_clean.cmake
.PHONY : renderer/CMakeFiles/NightlySubmit.dir/clean

renderer/CMakeFiles/NightlySubmit.dir/depend:
	cd /Users/willchambers/Projects/astral/src/renderer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/willchambers/Projects/astral/src /Users/willchambers/Projects/astral/src/renderer /Users/willchambers/Projects/astral/src/renderer /Users/willchambers/Projects/astral/src/renderer/renderer /Users/willchambers/Projects/astral/src/renderer/renderer/CMakeFiles/NightlySubmit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : renderer/CMakeFiles/NightlySubmit.dir/depend

