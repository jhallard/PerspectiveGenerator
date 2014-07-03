cmake_minimum_required (VERSION 2.6)
project (OBJ-Parser)


#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# Include Files
#########################################################
add_executable(OpenGL-OBJ-Parser src/Main/main.cpp src/MathHelp/MathHelp.cpp src/Rendering/Render.cpp src/IO/ProgramIO.cpp )

########################################################
# Linking & stuff
#########################################################
target_link_libraries(OpenGL-OBJ-Parser GL GLU IL GLEW glut X11 pthread /usr/local/lib/libassimp.so)
