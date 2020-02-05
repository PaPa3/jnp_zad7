cmake_minimum_required(VERSION 3.16)
project(jnp_zad7)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -O3 -fconcepts")

add_executable(example example.cc bmp.cc bmp.h color.cc color.h coordinate.cc coordinate.h functional.h images.h)