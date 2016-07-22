project(MortailCoilCmake)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)
aux_source_directory(./World SRC_LIST)
aux_source_directory(./Solver SRC_LIST)
aux_source_directory(./Solver/Data SRC_LIST)
set (CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} ${SRC_LIST})

