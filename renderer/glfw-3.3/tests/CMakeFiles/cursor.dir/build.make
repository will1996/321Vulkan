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

# Include any dependencies generated for this target.
include renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/depend.make

# Include the progress variables for this target.
include renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/progress.make

# Include the compile flags for this target's objects.
include renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/flags.make

renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/cursor.c.o: renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/flags.make
renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/cursor.c.o: glfw-3.3/tests/cursor.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/willchambers/Projects/astral/src/renderer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/cursor.c.o"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cursor.dir/cursor.c.o   -c /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/tests/cursor.c

renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/cursor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cursor.dir/cursor.c.i"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/tests/cursor.c > CMakeFiles/cursor.dir/cursor.c.i

renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/cursor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cursor.dir/cursor.c.s"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/tests/cursor.c -o CMakeFiles/cursor.dir/cursor.c.s

renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/__/deps/glad_gl.c.o: renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/flags.make
renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/__/deps/glad_gl.c.o: glfw-3.3/deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/willchambers/Projects/astral/src/renderer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/__/deps/glad_gl.c.o"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cursor.dir/__/deps/glad_gl.c.o   -c /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/glad_gl.c

renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cursor.dir/__/deps/glad_gl.c.i"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/glad_gl.c > CMakeFiles/cursor.dir/__/deps/glad_gl.c.i

renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cursor.dir/__/deps/glad_gl.c.s"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/glad_gl.c -o CMakeFiles/cursor.dir/__/deps/glad_gl.c.s

# Object files for target cursor
cursor_OBJECTS = \
"CMakeFiles/cursor.dir/cursor.c.o" \
"CMakeFiles/cursor.dir/__/deps/glad_gl.c.o"

# External object files for target cursor
cursor_EXTERNAL_OBJECTS =

renderer/glfw-3.3/tests/cursor: renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/cursor.c.o
renderer/glfw-3.3/tests/cursor: renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/__/deps/glad_gl.c.o
renderer/glfw-3.3/tests/cursor: renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/build.make
renderer/glfw-3.3/tests/cursor: renderer/glfw-3.3/src/libglfw3.a
renderer/glfw-3.3/tests/cursor: renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/willchambers/Projects/astral/src/renderer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable cursor"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cursor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/build: renderer/glfw-3.3/tests/cursor

.PHONY : renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/build

renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/clean:
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && $(CMAKE_COMMAND) -P CMakeFiles/cursor.dir/cmake_clean.cmake
.PHONY : renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/clean

renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/depend:
	cd /Users/willchambers/Projects/astral/src/renderer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/willchambers/Projects/astral/src /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/tests /Users/willchambers/Projects/astral/src/renderer /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : renderer/glfw-3.3/tests/CMakeFiles/cursor.dir/depend

