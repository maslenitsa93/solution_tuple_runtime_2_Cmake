cmake_minimum_required(VERSION 2.8)

project(ConsoleApplication1)

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
