cmake_minimum_required(VERSION 3.20)

# ---------------------------------------------------------------------
# PROJECT
# ---------------------------------------------------------------------
project(imgui)

# ---------------------------------------------------------------------
# SOURCE FILES
# ---------------------------------------------------------------------
# The source files are going to be compiled
set(IMGUI_HEADER_FILES
    imgui/imgui.h
    imgui/imconfig.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    )

set(IMGUI_SOURCE_FILES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_tables.cpp
    )

# ---------------------------------------------------------------------
# LIBRARY
# ---------------------------------------------------------------------
add_library(${PROJECT_NAME} ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})

# ---------------------------------------------------------------------
# INCLUDE DIRECTORIES
# ---------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/imgui)