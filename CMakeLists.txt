cmake_minimum_required (VERSION 3.8)

project ("parser-combinator")

set(CMAKE_CXX_STANDARD 20)

include(cmake/cpm.cmake)

enable_testing()
add_subdirectory("tests")