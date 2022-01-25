# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/e/watch/WeixinCMake_Project/WeixinCMake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build

# Include any dependencies generated for this target.
include weixin_port/CMakeFiles/WeixinChannel.dir/depend.make

# Include the progress variables for this target.
include weixin_port/CMakeFiles/WeixinChannel.dir/progress.make

# Include the compile flags for this target's objects.
include weixin_port/CMakeFiles/WeixinChannel.dir/flags.make

weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj: weixin_port/CMakeFiles/WeixinChannel.dir/flags.make
weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj: ../weixin_port/src/AdamClientChannelImpl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj"
	cd /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/weixin_port && /mnt/e/watch/WeixinCMake_Project/WeixinCMake/ToolChain/linux/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj -c /mnt/e/watch/WeixinCMake_Project/WeixinCMake/weixin_port/src/AdamClientChannelImpl.cpp

weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.i"
	cd /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/weixin_port && /mnt/e/watch/WeixinCMake_Project/WeixinCMake/ToolChain/linux/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/e/watch/WeixinCMake_Project/WeixinCMake/weixin_port/src/AdamClientChannelImpl.cpp > CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.i

weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.s"
	cd /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/weixin_port && /mnt/e/watch/WeixinCMake_Project/WeixinCMake/ToolChain/linux/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/e/watch/WeixinCMake_Project/WeixinCMake/weixin_port/src/AdamClientChannelImpl.cpp -o CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.s

weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj.requires:

.PHONY : weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj.requires

weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj.provides: weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj.requires
	$(MAKE) -f weixin_port/CMakeFiles/WeixinChannel.dir/build.make weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj.provides.build
.PHONY : weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj.provides

weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj.provides.build: weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj


# Object files for target WeixinChannel
WeixinChannel_OBJECTS = \
"CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj"

# External object files for target WeixinChannel
WeixinChannel_EXTERNAL_OBJECTS =

lib/libWeixinChannel.a: weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj
lib/libWeixinChannel.a: weixin_port/CMakeFiles/WeixinChannel.dir/build.make
lib/libWeixinChannel.a: weixin_port/CMakeFiles/WeixinChannel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../lib/libWeixinChannel.a"
	cd /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/weixin_port && $(CMAKE_COMMAND) -P CMakeFiles/WeixinChannel.dir/cmake_clean_target.cmake
	cd /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/weixin_port && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/WeixinChannel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
weixin_port/CMakeFiles/WeixinChannel.dir/build: lib/libWeixinChannel.a

.PHONY : weixin_port/CMakeFiles/WeixinChannel.dir/build

weixin_port/CMakeFiles/WeixinChannel.dir/requires: weixin_port/CMakeFiles/WeixinChannel.dir/src/AdamClientChannelImpl.cpp.obj.requires

.PHONY : weixin_port/CMakeFiles/WeixinChannel.dir/requires

weixin_port/CMakeFiles/WeixinChannel.dir/clean:
	cd /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/weixin_port && $(CMAKE_COMMAND) -P CMakeFiles/WeixinChannel.dir/cmake_clean.cmake
.PHONY : weixin_port/CMakeFiles/WeixinChannel.dir/clean

weixin_port/CMakeFiles/WeixinChannel.dir/depend:
	cd /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/watch/WeixinCMake_Project/WeixinCMake /mnt/e/watch/WeixinCMake_Project/WeixinCMake/weixin_port /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/weixin_port /mnt/e/watch/WeixinCMake_Project/WeixinCMake/build/weixin_port/CMakeFiles/WeixinChannel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : weixin_port/CMakeFiles/WeixinChannel.dir/depend
