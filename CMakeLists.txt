cmake_minimum_required(VERSION 3.10)

# Project name and language
project(MyGame C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -------------------------
# Add GLAD library
# -------------------------
add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)

# -------------------------
# Add GLFW library as imported static library
# -------------------------
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/lib/libglfw3.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include"
)


# -------------------------
# Sets the custom parts of the build 
# -------------------------

# Set the name of the executable file
set (EXE_NAME my_game)

# Set source Files
set(SOURCES
    src/render.c
    src/shader_utils.c
    src/main.c
)

# Set the custom include directories
set(INCLUDES
    include
)

# Toggles whether or not we want a console rerun cmake with -DHIDE_CONSOLE={ON or OFF}
option(HIDE_CONSOLE "Build GUI app without console window on Windows" OFF)

# -------------------------
# Add your executable
# -------------------------
if(HIDE_CONSOLE)
    add_executable(${EXE_NAME} WIN32 ${SOURCES})
else()
    add_executable(${EXE_NAME} ${SOURCES})
endif()

# Include directories
target_include_directories(${EXE_NAME} PUBLIC
    ${INCLUDES}
    extern/glad/include
    extern/glfw/include
    extern/cglm
)

# Link libraries
target_link_libraries(${EXE_NAME} PRIVATE
    glad
    glfw
    opengl32
    gdi32
)