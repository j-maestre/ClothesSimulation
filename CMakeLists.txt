cmake_minimum_required(VERSION 3.0)

# ---------------------------------------------------------------------
# SETUP
# ---------------------------------------------------------------------
# Basic setup for desktop application with RayLib
#


# ---------------------------------------------------------------------
# PROJECT
# ---------------------------------------------------------------------
# Set the project name
project(RayLib)


# ---------------------------------------------------------------------
# ADDITIONAL DIRECTORIES
# ---------------------------------------------------------------------
# The additional dirs is going to be copied on build or install
set(ADDITIONAL_DIRS)


# ---------------------------------------------------------------------
# DEPENDENCIES PATH
# ---------------------------------------------------------------------
# The path to the dependencies
# Get the path to raylib
set(RAYLIB_DIR ${PROJECT_SOURCE_DIR}/deps/raylib)
# Get the path to imgui
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/deps/imgui)
# Get the path to rlImGui
set(RLIMGUI_DIR ${PROJECT_SOURCE_DIR}/deps/rlImGui)
# Get thr path to the source
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)


# ---------------------------------------------------------------------
# SOURCE FILES
# ---------------------------------------------------------------------
# The source files are going to be compiled
# Auto detect source files and header files in the source directory
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/source/**.cpp)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/source/**.h)
# Detect the rlImGui files
file(GLOB RLIMGUI_SOURCE_FILES ${RLIMGUI_DIR}/**.cpp)
file(GLOB RLIMGUI_HEADER_FILES ${RLIMGUI_DIR}/**.h)
# Add rlImGui files
list(APPEND SOURCE_FILES ${RLIMGUI_SOURCE_FILES})
list(APPEND SOURCE_FILES ${RLIMGUI_HEADER_FILES})

# ---------------------------------------------------------------------
# INCLUDE DIRECTORIES
# ---------------------------------------------------------------------
# Set the additional include directories
set(INCLUDE_DIRS
    ${SOURCE_DIR}
    ${RLIMGUI_DIR}
    )


# ---------------------------------------------------------------------
# INSTALLATION PATH
# ---------------------------------------------------------------------
# Set the installation path
set(INSTALLATION_PATH ${PROJECT_SOURCE_DIR}/installation)


# ---------------------------------------------------------------------
# C++ CONFIGURATION
# ---------------------------------------------------------------------
# Set the C++ standard to 20
set(CMAKE_CXX_STANDARD 20)


# ---------------------------------------------------------------------
# DEPENDENCIES
# ---------------------------------------------------------------------
# Add dependencies are going to being built
# Set the raylib directory to being built
add_subdirectory(${RAYLIB_DIR} EXCLUDE_FROM_ALL)
# Set the imgui directory to being built
add_subdirectory(${IMGUI_DIR} EXCLUDE_FROM_ALL)


# ---------------------------------------------------------------------
# EXECUTABLE
# ---------------------------------------------------------------------
# Set the executable of the project
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})


# ---------------------------------------------------------------------
# INCLUDE DIRECTORIES SETUP
# ---------------------------------------------------------------------
# Setup the include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})


# ---------------------------------------------------------------------
# LINK OPTIONS
# ---------------------------------------------------------------------
# Sets the link libraries of the executable
# Link with raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
# Link with imgui
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)


# ---------------------------------------------------------------------
# POST-BUILD COMMANDS
# ---------------------------------------------------------------------
# Set the post-build commands
# Copy the files (after build) of the directories selected ass ADDITIONAL
foreach (dir ${ADDITIONAL_DIRS})
  add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/${dir}
  )
  file(GLOB_RECURSE files RELATIVE_PATH ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/${dir}/*)
  foreach (file ${files})
     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy ${file} ${PROJECT_BINARY_DIR}/${dir}
    )
  endforeach ()
endforeach ()


# ---------------------------------------------------------------------
# INSTALLATION CONFIGURATION
# ---------------------------------------------------------------------
# Setup the installation configuration
# Set the installation path
set(CMAKE_INSTALL_PREFIX ${INSTALLATION_PATH})

# Set the installation info
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION .
        )

# Copy the directories in the installation path
foreach (dir ${ADDITIONAL_DIRS})
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/${dir} DESTINATION ${INSTALLATION_PATH})
endforeach ()