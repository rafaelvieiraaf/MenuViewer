cmake_minimum_required(VERSION 3.10)

project(app)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES "./*.cpp")

file(GLOB HEADERS "./*.h" "./*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
