#ifndef SHADERFUNCTIONS_H_
#define SHADERFUNCTIONS_H_

#include "../Helper/MathHelp.h"
#include "../IO/ProgramIO.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

namespace Shaders
{

	char *textFileRead(char *fn);

	void printShaderInfoLog(GLuint obj);

	GLuint setupShaders(char * vertexfile, char * fragmentfile);

}// end namespace

#endif