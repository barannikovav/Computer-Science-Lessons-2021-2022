cmake_minimum_required(VERSION 3.10)

project(Computer_science_3_term)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wnarrowing -Wconversion -Wwrite-strings -Wcast-qual -Wundef -Wstrict-prototypes -Wbad-function-cast -Wreturn-type -std=gnu11 ")

add_subdirectory(01_stat)
add_subdirectory(02_write)
add_subdirectory(03_contentcopy)