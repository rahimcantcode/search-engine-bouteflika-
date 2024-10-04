cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# filesystem.h requires C++17
set(CMAKE_CXX_STANDARD 17)

# Copy sample data directory
file(COPY sample_data/ DESTINATION sample_data/)

# Show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

# Main Executable
# Ensure all source files are included in the executable
add_executable(supersearch main.cpp UI.cpp IndexHandler.cpp porter2_stemmer/porter2_stemmer.cpp)  # Add all .cpp files that are part of the main executable
target_include_directories(supersearch PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})  # Adjust if there are specific include directories

# Test executable for AVL Tree
add_executable(tests_AVL_Tree test_AVLTree.cpp AVLTree.h)
add_test(NAME TestAVLTree COMMAND tests_AVL_Tree)

# Test executable for IndexHandler
# add_executable(test_Index_Handler test_Index.cpp AVLTree.h document.h IndexHandler.cpp IndexHandler.h)
# add_test(NAME TestIndexHandler COMMAND test_Index_Handler)

# Test executable for Query
add_executable(test_Query test_Query.cpp Query.cpp Query.h AVLTree.h Utility/Utility.cpp Utility/Utility.h)
add_test(NAME TestQuery COMMAND test_Query)

# RapidJSON Example (if used)
add_executable(rapidJSONExample rapidJSONExample.cpp)
target_include_directories(rapidJSONExample PRIVATE rapidjson/)  # Ensure this directory is correct

# Turn off verbose makefile after configuration
set(CMAKE_VERBOSE_MAKEFILE OFF)
