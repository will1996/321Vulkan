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
include renderer/glfw-3.3/tests/CMakeFiles/windows.dir/depend.make

# Include the progress variables for this target.
include renderer/glfw-3.3/tests/CMakeFiles/windows.dir/progress.make

# Include the compile flags for this target's objects.
include renderer/glfw-3.3/tests/CMakeFiles/windows.dir/flags.make

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/windows.c.o: renderer/glfw-3.3/tests/CMakeFiles/windows.dir/flags.make
renderer/glfw-3.3/tests/CMakeFiles/windows.dir/windows.c.o: glfw-3.3/tests/windows.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/willchambers/Projects/astral/src/renderer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object renderer/glfw-3.3/tests/CMakeFiles/windows.dir/windows.c.o"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/windows.dir/windows.c.o   -c /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/tests/windows.c

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/windows.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/windows.dir/windows.c.i"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/tests/windows.c > CMakeFiles/windows.dir/windows.c.i

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/windows.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/windows.dir/windows.c.s"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/tests/windows.c -o CMakeFiles/windows.dir/windows.c.s

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o: renderer/glfw-3.3/tests/CMakeFiles/windows.dir/flags.make
renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o: glfw-3.3/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/willchambers/Projects/astral/src/renderer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/windows.dir/__/deps/getopt.c.o   -c /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/getopt.c

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/windows.dir/__/deps/getopt.c.i"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/getopt.c > CMakeFiles/windows.dir/__/deps/getopt.c.i

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/windows.dir/__/deps/getopt.c.s"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/getopt.c -o CMakeFiles/windows.dir/__/deps/getopt.c.s

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/glad_gl.c.o: renderer/glfw-3.3/tests/CMakeFiles/windows.dir/flags.make
renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/glad_gl.c.o: glfw-3.3/deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/willchambers/Projects/astral/src/renderer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/glad_gl.c.o"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/windows.dir/__/deps/glad_gl.c.o   -c /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/glad_gl.c

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/windows.dir/__/deps/glad_gl.c.i"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/glad_gl.c > CMakeFiles/windows.dir/__/deps/glad_gl.c.i

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/windows.dir/__/deps/glad_gl.c.s"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/deps/glad_gl.c -o CMakeFiles/windows.dir/__/deps/glad_gl.c.s

# Object files for target windows
windows_OBJECTS = \
"CMakeFiles/windows.dir/windows.c.o" \
"CMakeFiles/windows.dir/__/deps/getopt.c.o" \
"CMakeFiles/windows.dir/__/deps/glad_gl.c.o"

# External object files for target windows
windows_EXTERNAL_OBJECTS =

renderer/glfw-3.3/tests/windows.app/Contents/MacOS/windows: renderer/glfw-3.3/tests/CMakeFiles/windows.dir/windows.c.o
renderer/glfw-3.3/tests/windows.app/Contents/MacOS/windows: renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/getopt.c.o
renderer/glfw-3.3/tests/windows.app/Contents/MacOS/windows: renderer/glfw-3.3/tests/CMakeFiles/windows.dir/__/deps/glad_gl.c.o
renderer/glfw-3.3/tests/windows.app/Contents/MacOS/windows: renderer/glfw-3.3/tests/CMakeFiles/windows.dir/build.make
renderer/glfw-3.3/tests/windows.app/Contents/MacOS/windows: renderer/glfw-3.3/src/libglfw3.a
renderer/glfw-3.3/tests/windows.app/Contents/MacOS/windows: renderer/glfw-3.3/tests/CMakeFiles/windows.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/willchambers/Projects/astral/src/renderer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable windows.app/Contents/MacOS/windows"
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/windows.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
renderer/glfw-3.3/tests/CMakeFiles/windows.dir/build: renderer/glfw-3.3/tests/windows.app/Contents/MacOS/windows

.PHONY : renderer/glfw-3.3/tests/CMakeFiles/windows.dir/build

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/clean:
	cd /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests && $(CMAKE_COMMAND) -P CMakeFiles/windows.dir/cmake_clean.cmake
.PHONY : renderer/glfw-3.3/tests/CMakeFiles/windows.dir/clean

renderer/glfw-3.3/tests/CMakeFiles/windows.dir/depend:
	cd /Users/willchambers/Projects/astral/src/renderer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/willchambers/Projects/astral/src /Users/willchambers/Projects/astral/src/renderer/glfw-3.3/tests /Users/willchambers/Projects/astral/src/renderer /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests /Users/willchambers/Projects/astral/src/renderer/renderer/glfw-3.3/tests/CMakeFiles/windows.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : renderer/glfw-3.3/tests/CMakeFiles/windows.dir/depend

