cmake_minimum_required(VERSION 3.10)

project(copylib)

set(SOURCES copylib.c)
set(HEADERS copylib.h)
add_library(copylib ${SOURCES} ${HEADERS})
target_include_directories(copylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})