# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lidan/CLionProjects/cpurender

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lidan/CLionProjects/cpurender/cmake-build-release

# Include any dependencies generated for this target.
include external/cxxopts/test/CMakeFiles/options_test.dir/depend.make

# Include the progress variables for this target.
include external/cxxopts/test/CMakeFiles/options_test.dir/progress.make

# Include the compile flags for this target's objects.
include external/cxxopts/test/CMakeFiles/options_test.dir/flags.make

external/cxxopts/test/CMakeFiles/options_test.dir/main.cpp.o: external/cxxopts/test/CMakeFiles/options_test.dir/flags.make
external/cxxopts/test/CMakeFiles/options_test.dir/main.cpp.o: ../external/cxxopts/test/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lidan/CLionProjects/cpurender/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object external/cxxopts/test/CMakeFiles/options_test.dir/main.cpp.o"
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/options_test.dir/main.cpp.o -c /Users/lidan/CLionProjects/cpurender/external/cxxopts/test/main.cpp

external/cxxopts/test/CMakeFiles/options_test.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/options_test.dir/main.cpp.i"
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lidan/CLionProjects/cpurender/external/cxxopts/test/main.cpp > CMakeFiles/options_test.dir/main.cpp.i

external/cxxopts/test/CMakeFiles/options_test.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/options_test.dir/main.cpp.s"
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lidan/CLionProjects/cpurender/external/cxxopts/test/main.cpp -o CMakeFiles/options_test.dir/main.cpp.s

external/cxxopts/test/CMakeFiles/options_test.dir/options.cpp.o: external/cxxopts/test/CMakeFiles/options_test.dir/flags.make
external/cxxopts/test/CMakeFiles/options_test.dir/options.cpp.o: ../external/cxxopts/test/options.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lidan/CLionProjects/cpurender/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object external/cxxopts/test/CMakeFiles/options_test.dir/options.cpp.o"
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/options_test.dir/options.cpp.o -c /Users/lidan/CLionProjects/cpurender/external/cxxopts/test/options.cpp

external/cxxopts/test/CMakeFiles/options_test.dir/options.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/options_test.dir/options.cpp.i"
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lidan/CLionProjects/cpurender/external/cxxopts/test/options.cpp > CMakeFiles/options_test.dir/options.cpp.i

external/cxxopts/test/CMakeFiles/options_test.dir/options.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/options_test.dir/options.cpp.s"
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lidan/CLionProjects/cpurender/external/cxxopts/test/options.cpp -o CMakeFiles/options_test.dir/options.cpp.s

# Object files for target options_test
options_test_OBJECTS = \
"CMakeFiles/options_test.dir/main.cpp.o" \
"CMakeFiles/options_test.dir/options.cpp.o"

# External object files for target options_test
options_test_EXTERNAL_OBJECTS =

external/cxxopts/test/options_test: external/cxxopts/test/CMakeFiles/options_test.dir/main.cpp.o
external/cxxopts/test/options_test: external/cxxopts/test/CMakeFiles/options_test.dir/options.cpp.o
external/cxxopts/test/options_test: external/cxxopts/test/CMakeFiles/options_test.dir/build.make
external/cxxopts/test/options_test: external/cxxopts/test/CMakeFiles/options_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/lidan/CLionProjects/cpurender/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable options_test"
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/options_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/cxxopts/test/CMakeFiles/options_test.dir/build: external/cxxopts/test/options_test

.PHONY : external/cxxopts/test/CMakeFiles/options_test.dir/build

external/cxxopts/test/CMakeFiles/options_test.dir/clean:
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test && $(CMAKE_COMMAND) -P CMakeFiles/options_test.dir/cmake_clean.cmake
.PHONY : external/cxxopts/test/CMakeFiles/options_test.dir/clean

external/cxxopts/test/CMakeFiles/options_test.dir/depend:
	cd /Users/lidan/CLionProjects/cpurender/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lidan/CLionProjects/cpurender /Users/lidan/CLionProjects/cpurender/external/cxxopts/test /Users/lidan/CLionProjects/cpurender/cmake-build-release /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test/CMakeFiles/options_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/cxxopts/test/CMakeFiles/options_test.dir/depend

