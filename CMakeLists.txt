cmake_minimum_required(VERSION 3.16)
project(artscii_desktop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find needed packages
find_package(PNG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Qt-specific setup
qt_standard_project_setup()


# Include Directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/include_qt
)

# Source and Header Directories
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src_qt/*.cpp"
)
file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/include_qt/*.h"
)
add_executable(artscii_desktop ${SRC_FILES} ${HEADER_FILES})

# Link libraries from outside installs
target_link_libraries(artscii_desktop PRIVATE 
    PNG::PNG 
    Qt6::Widgets 
)

