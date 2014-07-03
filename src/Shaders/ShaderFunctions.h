
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>


namespace Shaders
{

	char *textFileRead(char *fn);

	GLuint setupShaders(char * vertexfile, char * fragment file);

	void printShaderInfoLog(GLuint obj);

}// end namespace