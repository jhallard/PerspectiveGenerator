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
            else
                std::cout << "fp NULL inside textFileRead" << std::endl;
        }
        else
            std::cout << "fn = null \n" << std::endl;
        return content;
    }




    GLuint setupShaders(char * vertexfile, char * fragmentfile) 
    {

        char *vs = NULL,*fs = NULL,*fs2 = NULL;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        if(vertexShader == 0 || fragmentShader == 0)
            std::cout << " Error with glCreateShader" << std::endl;

        vs = textFileRead(vertexfile);
        fs = textFileRead(fragmentfile);

        if(vs == NULL) 
            std::cout << "vs files null\n" << std::endl;
        if(fs == NULL)
            std::cout << "fs files null\n" << std::endl;

        const char * vv = vs;
        const char * ff = fs;

        glShaderSource(vertexShader, 1, &vv,NULL);
        glShaderSource(fragmentShader, 1, &ff,NULL);


        free(vs);free(fs);

        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        printShaderInfoLog(vertexShader);
        printShaderInfoLog(fragmentShader);

        program = glCreateProgram();
        glAttachShader(program,vertexShader);
        glAttachShader(program,fragmentShader);

        glBindFragDataLocation(program, 0, "output");

        glBindAttribLocation(program,vertexLoc,"position");
        glBindAttribLocation(program,normalLoc,"normal");
        glBindAttribLocation(program,texCoordLoc,"texCoord");

        glLinkProgram(program);
        glValidateProgram(program);
        IO::printProgramInfoLog(program);

        GLuint k = glGetUniformBlockIndex(program,"Matrices");
        glUniformBlockBinding(program, k, matricesUniLoc);
        glUniformBlockBinding(program, glGetUniformBlockIndex(program,"Material"), materialUniLoc);

        texUnit = glGetUniformLocation(program,"texUnit");

        return(program);
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