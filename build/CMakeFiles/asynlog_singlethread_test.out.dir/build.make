# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xiongfeng/FengLog

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiongfeng/FengLog/build

# Include any dependencies generated for this target.
include CMakeFiles/asynlog_singlethread_test.out.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/asynlog_singlethread_test.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/asynlog_singlethread_test.out.dir/flags.make

CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.o: CMakeFiles/asynlog_singlethread_test.out.dir/flags.make
CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.o: ../test/asynlog_singlethread_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiongfeng/FengLog/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.o -c /home/xiongfeng/FengLog/test/asynlog_singlethread_test.cpp

CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiongfeng/FengLog/test/asynlog_singlethread_test.cpp > CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.i

CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiongfeng/FengLog/test/asynlog_singlethread_test.cpp -o CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.s

# Object files for target asynlog_singlethread_test.out
asynlog_singlethread_test_out_OBJECTS = \
"CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.o"

# External object files for target asynlog_singlethread_test.out
asynlog_singlethread_test_out_EXTERNAL_OBJECTS =

../bin/release/asynlog_singlethread_test.out: CMakeFiles/asynlog_singlethread_test.out.dir/test/asynlog_singlethread_test.cpp.o
../bin/release/asynlog_singlethread_test.out: CMakeFiles/asynlog_singlethread_test.out.dir/build.make
../bin/release/asynlog_singlethread_test.out: ../lib/release/libfeng_log.so
../bin/release/asynlog_singlethread_test.out: CMakeFiles/asynlog_singlethread_test.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xiongfeng/FengLog/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/release/asynlog_singlethread_test.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/asynlog_singlethread_test.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/asynlog_singlethread_test.out.dir/build: ../bin/release/asynlog_singlethread_test.out

.PHONY : CMakeFiles/asynlog_singlethread_test.out.dir/build

CMakeFiles/asynlog_singlethread_test.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/asynlog_singlethread_test.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/asynlog_singlethread_test.out.dir/clean

CMakeFiles/asynlog_singlethread_test.out.dir/depend:
	cd /home/xiongfeng/FengLog/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiongfeng/FengLog /home/xiongfeng/FengLog /home/xiongfeng/FengLog/build /home/xiongfeng/FengLog/build /home/xiongfeng/FengLog/build/CMakeFiles/asynlog_singlethread_test.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/asynlog_singlethread_test.out.dir/depend

