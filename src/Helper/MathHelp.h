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



namespace MathHelp
{

	static inline float  DegToRad(float degrees);

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
	void setRotationMatrix(float *mat, float angle, float x, float y, float z) ;

} // end namespace