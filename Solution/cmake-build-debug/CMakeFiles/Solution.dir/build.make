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
CMAKE_COMMAND = /cygdrive/c/Users/M1820/.CLion2019.3/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/M1820/.CLion2019.3/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/d/desktop/CS/CS564/Assignments/Solution

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/d/desktop/CS/CS564/Assignments/Solution/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Solution.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Solution.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Solution.dir/flags.make

CMakeFiles/Solution.dir/main.cpp.o: CMakeFiles/Solution.dir/flags.make
CMakeFiles/Solution.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/d/desktop/CS/CS564/Assignments/Solution/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Solution.dir/main.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Solution.dir/main.cpp.o -c /cygdrive/d/desktop/CS/CS564/Assignments/Solution/main.cpp

CMakeFiles/Solution.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Solution.dir/main.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/d/desktop/CS/CS564/Assignments/Solution/main.cpp > CMakeFiles/Solution.dir/main.cpp.i

CMakeFiles/Solution.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Solution.dir/main.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/d/desktop/CS/CS564/Assignments/Solution/main.cpp -o CMakeFiles/Solution.dir/main.cpp.s

# Object files for target Solution
Solution_OBJECTS = \
"CMakeFiles/Solution.dir/main.cpp.o"

# External object files for target Solution
Solution_EXTERNAL_OBJECTS =

Solution.exe: CMakeFiles/Solution.dir/main.cpp.o
Solution.exe: CMakeFiles/Solution.dir/build.make
Solution.exe: CMakeFiles/Solution.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/d/desktop/CS/CS564/Assignments/Solution/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Solution.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Solution.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Solution.dir/build: Solution.exe

.PHONY : CMakeFiles/Solution.dir/build

CMakeFiles/Solution.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Solution.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Solution.dir/clean

CMakeFiles/Solution.dir/depend:
	cd /cygdrive/d/desktop/CS/CS564/Assignments/Solution/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/d/desktop/CS/CS564/Assignments/Solution /cygdrive/d/desktop/CS/CS564/Assignments/Solution /cygdrive/d/desktop/CS/CS564/Assignments/Solution/cmake-build-debug /cygdrive/d/desktop/CS/CS564/Assignments/Solution/cmake-build-debug /cygdrive/d/desktop/CS/CS564/Assignments/Solution/cmake-build-debug/CMakeFiles/Solution.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Solution.dir/depend

