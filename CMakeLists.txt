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

# Source Directories
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE SRC_QT_FILES
    "${CMAKE_SOURCE_DIR}/src_qt/*.cpp"
)
add_executable(artscii_desktop ${SRC_FILES} ${SRC_QT_FILES})

# Link libraries from outside installs
target_link_libraries(artscii_desktop PRIVATE 
    PNG::PNG 
    Qt6::Widgets 
)

