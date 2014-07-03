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

SET(OBJ-Parser_SOURCES src/Main/main.cpp src/Shaders/ShaderFunctions.cpp src/View/View.cpp src/Helper/MathHelp.cpp src/Rendering/Render.cpp src/IO/ProgramIO.cpp)
SET(OBJ-Parser_HEADERS ${QVFeatureFinder_HDRS} src/Shaders/ShaderFunctions.h src/View/View.h src/Helper/MathHelp.h src/Helper/HelperStructures.h src/Rendering/Render.h src/IO/ProgramIO.h)

#########################################################
# Include Files
#########################################################
add_executable(OpenGL-OBJ-Parser ${OBJ-Parser_SOURCES}
    ${OBJ-Parser_HEADERS_MOC}
    ${OBJ-Parser_FORMS_HEADERS} )

########################################################
# Linking & stuff
#########################################################
target_link_libraries(OpenGL-OBJ-Parser GL GLU IL GLEW glut X11 pthread /usr/local/lib/libassimp.so)
