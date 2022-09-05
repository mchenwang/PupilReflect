cmake_minimum_required(VERSION 3.12)
project(prefl)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/target/)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RUN_TEST ON)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (RUN_TEST)
    add_subdirectory(test)
endif()