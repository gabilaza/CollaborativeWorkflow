cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(getStringWindow LANGUAGES CXX)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)

add_executable(getStringWindow main.cpp entryWindow.cpp entryWindow.hpp)
target_link_libraries(getStringWindow PRIVATE ${GTKMM_LIBRARIES})
target_link_directories(getStringWindow PRIVATE ${GTKMM_LIBRARY_DIRS})
target_include_directories(getStringWindow PRIVATE ${GTKMM_INCLUDE_DIRS})
