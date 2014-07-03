#ifndef MATHHELP_H_
#define MATHHELP_H_

#include <cmath>
#include <iostream>
#include <vector>
#include <IL/il.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <string>
#include <exception>
#include <fstream>
#include <map>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

#define MatricesUniBufferSize sizeof(float) * 16 * 3
#define ProjMatrixOffset 0
#define ViewMatrixOffset sizeof(float) * 16
#define ModelMatrixOffset sizeof(float) * 16 * 2
#define MatrixSize sizeof(float) * 16

// Uniform Bindings Points
GLuint matricesUniLoc = 1, materialUniLoc = 2;

GLuint matricesUniBuffer;

// Vertex Attribute Locations
GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2;

// Program and Shader Identifiers
GLuint program, vertexShader, fragmentShader;

GLuint texUnit = 0;

// the global Assimp scene object
const aiScene* scene = NULL;

// scale factor for the model to fit in the window
float scaleFactor;


namespace MathHelp
{

	float  DegToRad(float degrees);

	void color4_to_float4(const aiColor4D *c, float f[4]);

	void set_float4(float f[4], float a, float b, float c, float d);

	// res = a cross b;
	void crossProduct( float *a, float *b, float *res);


	// Normalize a vec3
	void normalize(float *a);

	// sets the square matrix mat to the identity matrix,
	// size refers to the number of rows (or columns)
	void setIdentityMatrix( float *mat, int size);

	// a = a * b;
	void multMatrix(float *a, float *b);


	// Defines a transformation matrix mat with a translation
	void setTranslationMatrix(float *mat, float x, float y, float z);

	// Defines a transformation matrix mat with a scale
	void setScaleMatrix(float *mat, float sx, float sy, float sz);

	// Defines a transformation matrix mat with a rotation 
	// angle alpha and a rotation axis (x,y,z)
	void setRotationMatrix(float *mat, float angle, float x, float y, float z);

} // end namespace


#endif