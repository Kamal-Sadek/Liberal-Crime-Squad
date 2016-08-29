cmake_minimum_required (VERSION 3.5)
project (crimesquad)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

set (VERSION_MAJOR 4)
set (VERSION_MINOR 11)

configure_file (
  "${PROJECT_SOURCE_DIR}/LCSConfig.h.in"
  "${PROJECT_BINARY_DIR}/LCSConfig.h"
)

include_directories ("${PROJECT_BINARY_DIR}")
include_directories (src)
include_directories (src/*)

file (GLOB SOURCES "src/*.cpp")
file (GLOB SOURCES_DIR "src/*/*.cpp")

set (CMAKE_EXE_LINKER_FLAGS "-lncurses -lSDL2")

find_package (Curses REQUIRED)
find_package (SDL2 REQUIRED)
find_package (SDL2_Mixer REQUIRED)
include_directories (${CURSES_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIR})

add_executable (crimesquad ${SOURCES} ${SOURCES_DIR})

set_property (TARGET crimesquad PROPERTY CXX_STANDARD 11)

target_link_libraries (crimesquad ${CURSES_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})

