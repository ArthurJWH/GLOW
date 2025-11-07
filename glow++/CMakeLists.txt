cmake_minimum_required(VERSION 3.16)

# ---- Project ----
project(gui LANGUAGES CXX)

# ---- Global settings ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---- Find Qt ----
find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets)

# ---- FetchContent setup ----
include(FetchContent)

# Pull fmt directly from GitHub
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.0.2   # Use a stable release tag
)

# Download + make available
FetchContent_MakeAvailable(fmt)

# ---- Subdirectories ----
add_subdirectory(src)

add_custom_target(assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/bin/assets
)
add_dependencies(gui assets)
add_custom_target(info ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/docs
        ${CMAKE_BINARY_DIR}/bin/docs
)
add_dependencies(gui info)


