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
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/luis/Projects/solarbot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luis/Projects/solarbot/cmake-build-debug

# Utility rule file for Upload.

# Include the progress variables for this target.
include CMakeFiles/Upload.dir/progress.make

CMakeFiles/Upload:
	cd /home/luis/Projects/solarbot && /home/luis/Projects/solarbot/.venv/bin/platformio run -t upload

Upload: CMakeFiles/Upload
Upload: CMakeFiles/Upload.dir/build.make

.PHONY : Upload

# Rule to build all files generated by this target.
CMakeFiles/Upload.dir/build: Upload

.PHONY : CMakeFiles/Upload.dir/build

CMakeFiles/Upload.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Upload.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Upload.dir/clean

CMakeFiles/Upload.dir/depend:
	cd /home/luis/Projects/solarbot/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luis/Projects/solarbot /home/luis/Projects/solarbot /home/luis/Projects/solarbot/cmake-build-debug /home/luis/Projects/solarbot/cmake-build-debug /home/luis/Projects/solarbot/cmake-build-debug/CMakeFiles/Upload.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Upload.dir/depend

