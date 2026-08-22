cmake_minimum_required(VERSION 3.15)

project(shess)

target_compile_options( main PRIVATE
    -Wall
    -Werror
    -Wextra
    -O2
    -std=c99
)
