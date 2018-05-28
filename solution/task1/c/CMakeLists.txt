cmake_minimum_required(VERSION 2.8)

project(ClimateGen C)
enable_testing()
add_subdirectory(shim/cmocka)

set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic -std=c99")

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib_dir)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin_dir)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_LIBRARY_DIR})

# add_subdirectory(lib) 
# add_subdirectory(test)
# add_subdirectory(src)
include_directories(lib)

add_library(lib_climate lib/libclimate.c)

include_directories(shim/cmocka/include)
find_library(CMOKA_LIBRARY cmocka HINTS shim/cmocka/build/src)
add_executable(test_libclimate test/test_climate.c)
add_dependencies(test_libclimate test_libclimate)
add_test(test_libclimate test_libclimate)
target_link_libraries(test_libclimate lib_climate ${CMOCKA_LIBRARY})

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} lib_climate)