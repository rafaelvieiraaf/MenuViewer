cmake_minimum_required(VERSION 3.10)

project(app)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
set(CMAKE_CXX_STANDARD 23)

#find ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB SOURCES "./*.cpp")

file(GLOB HEADERS "./*.h" "./*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
