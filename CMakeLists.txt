cmake_minimum_required(VERSION 3.21.3)

# set the project name and VERSION
project(HanoiTowersSFML)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SFML_BASE_PATH "C:/SFML_64bit/SFML-2.5.1")

# search for SFML include files
find_library(SFML_LIBRARY sfml-graphics-s-d)
find_library(SFML_LIBRARY sfml-window-s-d)
find_library(SFML_LIBRARY sfml-system-s-d)

# add the executable
add_executable("${PROJECT_NAME}" HanoiTowersSFML.cpp)

add_compile_definitions(SFML_STATIC)

# add SFML include to project
target_include_directories("${PROJECT_NAME}" PUBLIC "${SFML_BASE_PATH}/include/")
target_link_directories("${PROJECT_NAME}" PUBLIC "${SFML_BASE_PATH}/lib/")

# Add SFML library to project
target_link_libraries("${PROJECT_NAME}" PUBLIC opengl32)
target_link_libraries("${PROJECT_NAME}" PUBLIC winmm)
target_link_libraries("${PROJECT_NAME}" PUBLIC gdi32)
target_link_libraries("${PROJECT_NAME}" PUBLIC sfml-graphics-s-d)
target_link_libraries("${PROJECT_NAME}" PUBLIC sfml-window-s-d)
target_link_libraries("${PROJECT_NAME}" PUBLIC sfml-system-s-d)