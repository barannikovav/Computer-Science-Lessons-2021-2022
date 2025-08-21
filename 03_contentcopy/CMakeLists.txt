cmake_minimum_required(VERSION 3.10)

project(03_contentcopy)

add_executable (03.1_copy.exe 03.1_copy.c)
add_executable (03.2_pcopy.exe 03.2_pcopy.c)
add_executable(03.s_extcopy.exe 03.s_extcopy.c)
add_subdirectory(copylib)
target_link_libraries(03.s_extcopy.exe copylib)