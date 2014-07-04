#ifndef RENDER_H_
#define RENDER_H_

#include "../Helper/MathHelp.h"
#include "../Helper/HelperStructures.h"
#include "../View/View.h"


extern std::vector<struct Helper::MyMesh> myMeshes;

// For push and pop matrix
extern std::vector<float *> matrixStack;

// Camera Position
extern float camera[3];

// mesh tranlation
extern float translation[3];

// Model Matrix (part of the OpenGL Model View Matrix)
extern float modelMatrix[16];

// Frame counting and FPS computation
extern long time1;
extern long timebase;
extern long frame;

extern char s[32];

namespace Render 
{

	void timer(int);

	void popMatrix();

	void pushMatrix();

	void genVAOsAndUniformBuffer(std::map<std::string, GLuint> * textureIdMap);

	void recursive_render (const aiNode* nd);

	void renderScene(void);

	void clearMeshes();

	void setModelMatrix();

	// The equivalent to glTranslate applied to the model matrix
	void translate(float x, float y, float z);

	// The equivalent to glRotate applied to the model matrix
	void rotate(float angle, float x, float y, float z);

	// The equivalent to glScale applied to the model matrix
	void scale(float x, float y, float z);

}

#endif