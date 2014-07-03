
#define MatricesUniBufferSize sizeof(float) * 16 * 3
#define ProjMatrixOffset 0
#define ViewMatrixOffset sizeof(float) * 16
#define ModelMatrixOffset sizeof(float) * 16 * 2
#define MatrixSize sizeof(float) * 16

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <exception>
#include <math.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "HelperStructures.h"
#include "MathHelp.h"



std::vector<struct Helper::MyMesh> myMeshes;

// Model Matrix (part of the OpenGL Model View Matrix)
float modelMatrix[16];

// For push and pop matrix
std::vector<float *> matrixStack;

// Uniform Bindings Points
GLuint matricesUniLoc = 1, materialUniLoc = 2;

// Uniform Buffer for Matrices
// this buffer will contain 3 matrices: projection, view and model
// each matrix is a float array with 16 components
GLuint matricesUniBuffer;


// Program and Shader Identifiers
GLuint program, vertexShader, fragmentShader;

// Shader Names
char *vertexFileName;// = "../dirlightdiffambpix.vert";
char *fragmentFileName;// = "../dirlightdiffambpix.frag";

// the global Assimp scene object
const aiScene* scene = NULL;

// scale factor for the model to fit in the window
float scaleFactor;

std::map<std::string, GLuint> * textureIdMap = new std::map<std::string, Gluint>();	

static const std::string modelname;// = "../14db49e526f340dfba81c4a2da23c716/14db49e526f340dfba81c4a2da23c716.obj";

// Camera Position
float camX = 0, camY = 0, camZ = 0;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

float transx = 0, transy = 0, transz = 0;

// Camera Spherical Coordinates
float alpha = 0.0f, beta = 0.0f;
float r = 5.0f;


int init();

int main(int argc, char **argv) {

//  GLUT initialization
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE );

	glutInitContextVersion (3, 3);
	glutInitContextFlags (GLUT_COMPATIBILITY_PROFILE );

	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,800);
	glutCreateWindow("Lighthouse3D - Assimp Demo");
		

//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	
	glutMouseWheelFunc ( mouseWheel ) ;

//	Init GLEW
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
   printf ("Vendor: %s\n", glGetString (GL_VENDOR));
   printf ("Renderer: %s\n", glGetString (GL_RENDERER));
   printf ("Version: %s\n", glGetString (GL_VERSION));
   printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));


   // return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);


	//  GLUT main loop
	glutMainLoop();

	// cleaning up
	textureIdMap->clear();  

	// clear myMeshes stuff
	for (unsigned int i = 0; i < myMeshes.size(); ++i) {
			
		glDeleteVertexArrays(1,&(myMeshes[i].vao));
		glDeleteTextures(1,&(myMeshes[i].texIndex));
		glDeleteBuffers(1,&(myMeshes[i].uniformBlockIndex));
	}
	// delete buffers
	glDeleteBuffers(1,&matricesUniBuffer);

	return(0);
}



int init()					 
{
	if (!Import3DFromFile(modelname)) 
		return(0);

	LoadGLTextures(scene, textureIdMap);

	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) glutGetProcAddress("glGetUniformBlockIndex");
	glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) glutGetProcAddress("glUniformBlockBinding");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) glutGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glutGetProcAddress("glBindVertexArray");
	glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) glutGetProcAddress("glBindBufferRange");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) glutGetProcAddress("glDeleteVertexArrays");


	program = setupShaders();

	genVAOsAndUniformBuffer(scene);

	glEnable(GL_DEPTH_TEST);		
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);


	//
	// Uniform Block
	//
	glGenBuffers(1,&matricesUniBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
	glBufferData(GL_UNIFORM_BUFFER, MatricesUniBufferSize,NULL,GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, matricesUniLoc, matricesUniBuffer, 0, MatricesUniBufferSize);	//setUniforms();
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glEnable(GL_MULTISAMPLE);





	return true;					
}
