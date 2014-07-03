#ifndef RENDER_H_
#define RENDER_H_

#include "../Helper/MathHelp.h"
#include "../Helper/HelperStructures.h"
#include "../View/View.h"

namespace Render 
{

	std::vector<struct Helper::MyMesh> myMeshes;

	// For push and pop matrix
	std::vector<float *> matrixStack;

	// Camera Position
	float camera[3] = {0, 0, 0};

	// mesh tranlation
	float translation[3] = {0, 0, 0};

	// Model Matrix (part of the OpenGL Model View Matrix)
	float modelMatrix[16];

	// Frame counting and FPS computation
	long time1,timebase = 0,frame = 0;
	char s[32];

	void popMatrix();

	void pushMatrix();

	void genVAOsAndUniformBuffer(const aiScene *sc,  std::map<std::string, GLuint> * textureIdMap);

	void recursive_render (const aiScene *sc, const aiNode* nd);

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