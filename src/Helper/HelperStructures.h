#include <IL/il.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

namespace Helper
{
	// Information to render each assimp node
	struct MyMesh{

		GLuint vao;
		GLuint texIndex;
		GLuint uniformBlockIndex;
		int numFaces;
	};

	// This is for a shader uniform block
	struct MyMaterial{

		float diffuse[4];
		float ambient[4];
		float specular[4];
		float emissive[4];
		float shininess;
		int texCount;
	};
}