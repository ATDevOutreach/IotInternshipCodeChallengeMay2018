cmake_minimum_required(VERSION 2.8)

project(LibClimate cmocka-library )

add_subdirectory(shim/cmoaka)
add_subdirectory(test)

enable_testing()

set(CMAKE_BUILD_TYPE Release)
set(CMOCKA_SHARED_LIBRARY cmocka-library)

set(CMAKE_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib_dir)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin_dir)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_LIBRARY_DIR})

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
include_directories("${LIB_DIR}") 
set(APP_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories("${APP_DIR}")
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
include_directories("${TEST_DIR}")

set(APP_SRC ${APP_DIR}/main.c)
set(LIB_SRC ${LIB_DIR}/libclimate.c)
file(GLOB TEST_SRC ${TEST_DIR}/*.c)

add_library(${PROJECT_NAME} SHARED ${LIB_SRC})
set(SHARED_LIB ${PROJECT_NAME})

include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(${SHARED_LIB}
    BASE_NAME ${SHARED_LIB}
    EXPORT_MACRO_NAME ${SHARED_LIB}_EXPORTS
    EXPORT_FILE_NAME ${SHARED_LIB}_EXPORTS.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)
  
include_directories("${PROJECT_SOURCE_DIR}/shim/cmocka/include")
add_subdirectory(shim/cmocka)
foreach(test_Src ${TEST_SRC})
get_filename_component(testName ${testSrc} NAME_WE)
add_test(${testName} ${TEST_SRC})
add_executable(${testName} ${TEST_SRC}) 
add_dependencies(${testName} ${CMOCKA_SHARED_LIBRARY} ${SHARED_LIB})
target_link_libraries(${tesName} ${CMOCKA_SHARED_LIBRARY} ${SHARED_LIB}) 
endforeach(test_Src)
   
add_executable(${PROJECT_NAME} ${APP_SRC})
target_link_libraries(${PROJECT_NAME} ${SHARED_LIB})