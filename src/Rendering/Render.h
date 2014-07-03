#include "MathHelp.h"
#include "HelperStructures.h"

using namespace Render 
{
	// Vertex Attribute Locations
	GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2;

	// Model Matrix (part of the OpenGL Model View Matrix)
	float modelMatrix[16];
	
	// The sampler uniform for textured models
	// we are assuming a single texture so this will
	//always be texture unit 0
	GLuint texUnit = 0;

	void genVAOsAndUniformBuffer(const aiScene *sc);

	void recursive_render (const aiScene *sc, const aiNode* nd);

	void renderScene(void);
}