cmake_minimum_required(VERSION 3.17)

project(mjc284_fsm_processor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_executable(assembler src/assembler.cpp)

add_executable(disassembler src/disassembler.cpp)

add_executable(compiler src/compiler.cpp)

add_executable(mif_generator src/mif_generator.cpp)
