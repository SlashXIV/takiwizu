# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/build

# Include any dependencies generated for this target.
include CMakeFiles/game_test_rdubernard.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/game_test_rdubernard.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/game_test_rdubernard.dir/flags.make

CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.o: CMakeFiles/game_test_rdubernard.dir/flags.make
CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.o: ../game_test_rdubernard.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.o -c /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/game_test_rdubernard.c

CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/game_test_rdubernard.c > CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.i

CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/game_test_rdubernard.c -o CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.s

# Object files for target game_test_rdubernard
game_test_rdubernard_OBJECTS = \
"CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.o"

# External object files for target game_test_rdubernard
game_test_rdubernard_EXTERNAL_OBJECTS =

game_test_rdubernard: CMakeFiles/game_test_rdubernard.dir/game_test_rdubernard.c.o
game_test_rdubernard: CMakeFiles/game_test_rdubernard.dir/build.make
game_test_rdubernard: CMakeFiles/game_test_rdubernard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable game_test_rdubernard"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/game_test_rdubernard.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/game_test_rdubernard.dir/build: game_test_rdubernard

.PHONY : CMakeFiles/game_test_rdubernard.dir/build

CMakeFiles/game_test_rdubernard.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/game_test_rdubernard.dir/cmake_clean.cmake
.PHONY : CMakeFiles/game_test_rdubernard.dir/clean

CMakeFiles/game_test_rdubernard.dir/depend:
	cd /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/build /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/build /autofs/unitytravail/travail/ibensaad/projet-t/Takuzu/takuzu-06a/build/CMakeFiles/game_test_rdubernard.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/game_test_rdubernard.dir/depend

