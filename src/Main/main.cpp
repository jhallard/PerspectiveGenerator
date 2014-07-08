
#include <IL/il.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "assimp/Importer.hpp"  //OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <exception>
#include <math.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <iostream>

//#include "../Helper/MathHelp.h"
//#include "../Helper/HelperStructures.h"
#include "../View/View.h"
#include "../Rendering/Render.h"
#include "../Shaders/ShaderFunctions.h"
#include "../IO/ProgramIO.h"


int init();

void timer(int)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(.2, .2, .5);//translation[0], translation[1], translation[2]);
    std::cout << "timer function \n" << std::endl;
}

int main(int argc, char **argv) 
{

//  GLUT initialization
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);//|GLUT_MULTISAMPLE );

    glutInitContextVersion (3, 3);
    glutInitContextFlags (GLUT_COMPATIBILITY_PROFILE );

    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,800);
    glutCreateWindow("OBJParser Demo");
        

//  Callback Registration
    glutDisplayFunc(Render::renderScene);
    glutReshapeFunc(IO::changeSize);
    glutIdleFunc(Render::renderScene);

//  Mouse and Keyboard Callbacks
    glutKeyboardFunc(IO::processKeys);
    glutMouseFunc(IO::processMouseButtons);
    glutMotionFunc(IO::processMouseMotion);
    glutMouseWheelFunc ( IO::mouseWheel ) ;


    if(argc == 2)
        IO::parseInputFile(argv[1]);
    else
        std::cout << "Wrong number of command line arguments\n" << std::endl;
//  Init GLEW
    //glewExperimental = GL_TRUE;
    glewInit();
    if (glewIsSupported("GL_VERSION_3_3"))
        printf("Ready for OpenGL 3.3\n");
    else {
        printf("OpenGL 3.3 not supported\n");
        return(1);
    }

    //  Init the app (load model and textures) and OpenGL
    if (!init())
    {
        printf("Could not Load the Model\n");
        return 0;
    }
   // printf ("Vendor: %s\n", glGetString (GL_VENDOR));
   // printf ("Renderer: %s\n", glGetString (GL_RENDERER));
   // printf ("Version: %s\n", glGetString (GL_VERSION));
   // printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));


    // return from main loop
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    //glutTimerFunc(1600, timer, 1);
    glutMainLoop();

    // cleaning up
    textureIdMap.clear();  

    Render::clearMeshes();

    // delete buffers
    glDeleteBuffers(1,&matricesUniBuffer);

    return(0);
}



int init()                   
{
    if (!IO::Import3DFromFile(modelname))
    {
        std::cout << "Failed 3d model import\n\n"<< std::endl; 
        return(0);
    }
    IO::LoadGLTextures();

    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) glutGetProcAddress("glGetUniformBlockIndex");
    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) glutGetProcAddress("glUniformBlockBinding");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) glutGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glutGetProcAddress("glBindVertexArray");
    glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) glutGetProcAddress("glBindBufferRange");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) glutGetProcAddress("glDeleteVertexArrays");


    program = Shaders::setupShaders(vertexfile, fragmentfile);

    Render::genVAOsAndUniformBuffer();

    glEnable(GL_DEPTH_TEST);        
    glClearColor(1.0f, 0.0f, 1.0f, 0.0f);


    //
    // Uniform Block
    //
    glGenBuffers(1,&matricesUniBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
    glBufferData(GL_UNIFORM_BUFFER, MatricesUniBufferSize,NULL,GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, matricesUniLoc, matricesUniBuffer, 0, MatricesUniBufferSize);  
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    glEnable(GL_MULTISAMPLE);


    return true;                    
}
