#include "HelperStructures.h"
#include "MathHelp.h"
#include "View.h"

namespace IO
{

	bool Import3DFromFile( const std::string& pFile, const aiScene* scene);

	int LoadGLTextures(const aiScene* scene, std::map<std::string, GLuint> * textureIdMap);

	void printProgramInfoLog(GLuint obj);

	void changeSize(int w, int h);

	void processKeys(unsigned char key, int xx, int yy);

	void processMouseButtons(int button, int state, int xx, int yy);

	#define printOpenGLError() printOglError(__FILE__, __LINE__)
	int printOglError(char *file, int line)

} // end namespace IO
