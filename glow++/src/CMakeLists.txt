cmake_minimum_required(VERSION 3.16)

# ---- Source files ----
set(SOURCES
    main.cpp
    ui/glowwindow.cpp
    ui/glowwindow.ui
    widgets/canvas.cpp
    widgets/dataframe.cpp
    widgets/filedrop.cpp
)
set(HEADERS
    ui/glowwindow.h
    widgets/canvas.h
    widgets/dataframe.h
    widgets/filedrop.h
)

# ---- Executable target ----
add_executable(gui ${SOURCES} ${HEADERS})
set_target_properties(gui PROPERTIES WIN32_EXECUTABLE TRUE)

# ---- Include directories ----
target_include_directories(gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
)

# ---- Link libraries ----
target_link_libraries(gui PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
    fmt::fmt-header-only
)

# ---- Output directory (optional) ----
set_target_properties(gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
