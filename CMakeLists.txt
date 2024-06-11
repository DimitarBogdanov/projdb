cmake_minimum_required(VERSION "3.0.0")
project("projdb")

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(projdb ${SOURCES})