CPMAddPackage("gh:doctest/doctest@2.4.11")

file(GLOB_RECURSE UNITTEST_FILES "*.cpp")
add_executable(unittest ${UNITTEST_FILES})

target_link_libraries(unittest doctest::doctest)
set_target_properties(unittest PROPERTIES CXX_STANDARD 23)
target_include_directories(unittest PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(unittest INTERFACE ${doctest_SOURCE_DIR})

enable_testing()

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(unittest)