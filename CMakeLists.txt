cmake_minimum_required(VERSION 3.16)

project(cpp_qt-gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

qt_standard_project_setup()

add_executable(cpp_qt-gui src/main.cpp src/utils.cpp)

target_include_directories(cpp_qt-gui PRIVATE include)

target_link_libraries(cpp_qt-gui
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
