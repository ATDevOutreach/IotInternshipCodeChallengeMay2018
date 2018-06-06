cmake_minimum_required(VERSION 3.1)
project(MqttConsumer)

set(SOURCES src/main.cc)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/include/mraa/api")
add_subdirectory(include/mraa)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}  mraa)
