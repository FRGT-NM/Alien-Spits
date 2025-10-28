cmake_minimum_required(VERSION 3.5)
project(QT_Practice)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(QT_Practice QT_Practice.cpp)

target_link_libraries(QT_Practice Qt5::Widgets)
