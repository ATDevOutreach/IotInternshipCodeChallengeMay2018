# CMake Version
cmake_minimum_required(VERSION 2.8)

# Project Name
project(LibClimate CXX)

enable_testing()

# Build for Release
set(CMAKE_BUILD_TYPE Release)
message(WARNING "A ${CMAKE_BUILD_TYPE} build configuration is detected")

# Where to throw Binaries
set(CMAKE_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/libs)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/out)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_LIBRARY_DIR})

# Add directories
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib)
include_directories("${PROJECT_INCLUDE_DIR}")

# Optional args for running Google Test
option (DO_TEST "Run Google Test" ON) 

# Add Source Files
set(PROJECT_SRCS  src/main.cc )

# Add Library Source Files
set(LIB_SRC  lib/libclimate.cc )

# Build shared Libs
add_library(${PROJECT_NAME}_shared SHARED ${LIB_SRC})
set(SHARED_LIB ${PROJECT_NAME}_shared)

# Fix symbol visibility
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(${SHARED_LIB}
    BASE_NAME ${SHARED_LIB}
    EXPORT_MACRO_NAME ${SHARED_LIB}_EXPORTS
    EXPORT_FILE_NAME ${SHARED_LIB}_EXPORTS.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

    
if(DO_TEST)
    include_directories("${PROJECT_SOURCE_DIR}/include/googletest/googletest/include")
    add_subdirectory(include/googletest)
    set(TEST_LIBS_FLAGS ${CMAKE_LIBRARY_DIR}/libgtest_main.a m gtest pthread ${SHARED_LIB})
    add_executable(test_libclimate test/test_libclimate.cc) 
    add_dependencies(test_libclimate gtest_main)
    target_link_libraries(test_libclimate ${TEST_LIBS_FLAGS})
endif (DO_TEST)

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})
target_link_libraries(${PROJECT_NAME} ${SHARED_LIB})


