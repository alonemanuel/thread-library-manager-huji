# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /usr/local/APP/jetbrains/clion/2018.2.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /usr/local/APP/jetbrains/clion/2018.2.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cs/usr/alonemanuel/OS/ex2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cs/usr/alonemanuel/OS/ex2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ex2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ex2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ex2.dir/flags.make

CMakeFiles/ex2.dir/uthreads.cpp.o: CMakeFiles/ex2.dir/flags.make
CMakeFiles/ex2.dir/uthreads.cpp.o: ../uthreads.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cs/usr/alonemanuel/OS/ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ex2.dir/uthreads.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ex2.dir/uthreads.cpp.o -c /cs/usr/alonemanuel/OS/ex2/uthreads.cpp

CMakeFiles/ex2.dir/uthreads.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex2.dir/uthreads.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cs/usr/alonemanuel/OS/ex2/uthreads.cpp > CMakeFiles/ex2.dir/uthreads.cpp.i

CMakeFiles/ex2.dir/uthreads.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex2.dir/uthreads.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cs/usr/alonemanuel/OS/ex2/uthreads.cpp -o CMakeFiles/ex2.dir/uthreads.cpp.s

CMakeFiles/ex2.dir/sleeping_threads_list.cpp.o: CMakeFiles/ex2.dir/flags.make
CMakeFiles/ex2.dir/sleeping_threads_list.cpp.o: ../sleeping_threads_list.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cs/usr/alonemanuel/OS/ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ex2.dir/sleeping_threads_list.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ex2.dir/sleeping_threads_list.cpp.o -c /cs/usr/alonemanuel/OS/ex2/sleeping_threads_list.cpp

CMakeFiles/ex2.dir/sleeping_threads_list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex2.dir/sleeping_threads_list.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cs/usr/alonemanuel/OS/ex2/sleeping_threads_list.cpp > CMakeFiles/ex2.dir/sleeping_threads_list.cpp.i

CMakeFiles/ex2.dir/sleeping_threads_list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex2.dir/sleeping_threads_list.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cs/usr/alonemanuel/OS/ex2/sleeping_threads_list.cpp -o CMakeFiles/ex2.dir/sleeping_threads_list.cpp.s

CMakeFiles/ex2.dir/thread.cpp.o: CMakeFiles/ex2.dir/flags.make
CMakeFiles/ex2.dir/thread.cpp.o: ../thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cs/usr/alonemanuel/OS/ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ex2.dir/thread.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ex2.dir/thread.cpp.o -c /cs/usr/alonemanuel/OS/ex2/thread.cpp

CMakeFiles/ex2.dir/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex2.dir/thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cs/usr/alonemanuel/OS/ex2/thread.cpp > CMakeFiles/ex2.dir/thread.cpp.i

CMakeFiles/ex2.dir/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex2.dir/thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cs/usr/alonemanuel/OS/ex2/thread.cpp -o CMakeFiles/ex2.dir/thread.cpp.s

CMakeFiles/ex2.dir/t4.cpp.o: CMakeFiles/ex2.dir/flags.make
CMakeFiles/ex2.dir/t4.cpp.o: ../t4.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cs/usr/alonemanuel/OS/ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ex2.dir/t4.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ex2.dir/t4.cpp.o -c /cs/usr/alonemanuel/OS/ex2/t4.cpp

CMakeFiles/ex2.dir/t4.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex2.dir/t4.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cs/usr/alonemanuel/OS/ex2/t4.cpp > CMakeFiles/ex2.dir/t4.cpp.i

CMakeFiles/ex2.dir/t4.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex2.dir/t4.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cs/usr/alonemanuel/OS/ex2/t4.cpp -o CMakeFiles/ex2.dir/t4.cpp.s

# Object files for target ex2
ex2_OBJECTS = \
"CMakeFiles/ex2.dir/uthreads.cpp.o" \
"CMakeFiles/ex2.dir/sleeping_threads_list.cpp.o" \
"CMakeFiles/ex2.dir/thread.cpp.o" \
"CMakeFiles/ex2.dir/t4.cpp.o"

# External object files for target ex2
ex2_EXTERNAL_OBJECTS =

ex2: CMakeFiles/ex2.dir/uthreads.cpp.o
ex2: CMakeFiles/ex2.dir/sleeping_threads_list.cpp.o
ex2: CMakeFiles/ex2.dir/thread.cpp.o
ex2: CMakeFiles/ex2.dir/t4.cpp.o
ex2: CMakeFiles/ex2.dir/build.make
ex2: CMakeFiles/ex2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cs/usr/alonemanuel/OS/ex2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ex2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ex2.dir/build: ex2

.PHONY : CMakeFiles/ex2.dir/build

CMakeFiles/ex2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ex2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ex2.dir/clean

CMakeFiles/ex2.dir/depend:
	cd /cs/usr/alonemanuel/OS/ex2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cs/usr/alonemanuel/OS/ex2 /cs/usr/alonemanuel/OS/ex2 /cs/usr/alonemanuel/OS/ex2/cmake-build-debug /cs/usr/alonemanuel/OS/ex2/cmake-build-debug /cs/usr/alonemanuel/OS/ex2/cmake-build-debug/CMakeFiles/ex2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ex2.dir/depend

