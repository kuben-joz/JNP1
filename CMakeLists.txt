cmake_minimum_required(VERSION 3.13)
project(JNP1_2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
#set(SOURCE_FILES poset.cc poset.h)
add_library(libposet poset.cc poset.h)

##########################
#Compile C++ test
##########################
add_executable(cpptest cpptest.cc)
target_link_libraries(cpptest libposet)

add_executable(ctest ctest.c)
target_link_libraries(ctest libposet)

add_executable(poset_example1 poset_example1.c)
target_link_libraries(poset_example1 libposet)
