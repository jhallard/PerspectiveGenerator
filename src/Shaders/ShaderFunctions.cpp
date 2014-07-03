#include "ShaderFunctions.h"


namespace Shaders
{

	void printShaderInfoLog(GLuint obj);
	GLuint setupShaders(char * vertexfile, char * fragmentfile);
	char *textFileRead(char *fn);

	char *textFileRead(char *fn) 
	{
		FILE *fp;
		char *content = NULL;
		int count=0;

		if (fn != NULL)
		{
			fp = fopen(fn,"rt");

			if (fp != NULL) 
			{
	      		fseek(fp, 0, SEEK_END);
	      		count = ftell(fp);
	      		rewind(fp);

				if (count > 0)
				{
					content = (char *)malloc(sizeof(char) * (count+1));
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
		}
		else
		{
			std::cout << "fn = null \n" << std::endl;
		}
		return content;
	}




	GLuint setupShaders(char * vertexfile, char * fragmentfile) 
	{

		char *vs = NULL,*fs = NULL,*fs2 = NULL;

		GLuint p,v,f;

		v = glCreateShader(GL_VERTEX_SHADER);
		f = glCreateShader(GL_FRAGMENT_SHADER);

		vs = textFileRead(vertexfile);
		fs = textFileRead(fragmentfile);

		if(vs == NULL) 
			std::cout << "vs files null\n" << std::endl;
		if((fs == NULL))
			std::cout << "fs files null\n" << std::endl;

		const char * vv = vs;
		const char * ff = fs;

		glShaderSource(v, 1, &vv,NULL);
		glShaderSource(f, 1, &ff,NULL);


		free(vs);free(fs);

		glCompileShader(v);
		glCompileShader(f);

		printShaderInfoLog(v);
		printShaderInfoLog(f);

		p = glCreateProgram();
		glAttachShader(p,v);
		glAttachShader(p,f);

		glBindFragDataLocation(p, 0, "output");

		glBindAttribLocation(p,vertexLoc,"position");
		glBindAttribLocation(p,normalLoc,"normal");
		glBindAttribLocation(p,texCoordLoc,"texCoord");

		glLinkProgram(p);
		glValidateProgram(p);
		IO::printProgramInfoLog(p);

		program = p;
		vertexShader = v;
		fragmentShader = f;
		
		GLuint k = glGetUniformBlockIndex(p,"Matrices");
		glUniformBlockBinding(p, k, matricesUniLoc);
		glUniformBlockBinding(p, glGetUniformBlockIndex(p,"Material"), materialUniLoc);

		texUnit = glGetUniformLocation(p,"texUnit");

		return(p);
	}


	void printShaderInfoLog(GLuint obj)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	    if (infologLength > 0)
	    {
	        infoLog = (char *)malloc(infologLength);
	        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
	        free(infoLog);
	    }
	}

}// end namespace