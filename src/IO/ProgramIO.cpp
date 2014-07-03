#include "ProgramIO.h"

namespace IO
{

	// Create an instance of the Importer class
	Assimp::Importer importer;

	bool Import3DFromFile( const std::string& pFile, const aiScene* scene)
	{
		std::ifstream fin(pFile.c_str());
		if(!fin.fail()) {
			fin.close();
			std::cout << "found model\n\n";
		}
		else
		{
			printf("Couldn't open file: %s\n", pFile.c_str());
			printf("%s\n", importer.GetErrorString());
			return false;
		}
		scene = importer.ReadFile( pFile.c_str(), aiProcessPreset_TargetRealtime_Quality);
		if((scene == NULL))
		{
			printf("%s\n", importer.GetErrorString());
			return false;
		}
		// Now we can access the file's contents.
		printf("Import of scene %s succeeded.", pFile.c_str());
		std::cout << std::endl;

		aiVector3D scene_min, scene_max, scene_center;
		get_bounding_box(&scene_min, &scene_max);
		float tmp;
		tmp = scene_max.x-scene_min.x;
		tmp = scene_max.y - scene_min.y > tmp?scene_max.y - scene_min.y:tmp;
		tmp = scene_max.z - scene_min.z > tmp?scene_max.z - scene_min.z:tmp;
		scaleFactor = 1.f / tmp;


		// We're done. Everything will be cleaned up by the importer destructor
		return true;
	}


	int LoadGLTextures(const aiScene* scene, std::map<std::string, GLuint> * textureIdMap)
	{
		ILboolean success;

		/* initialization of DevIL */
		ilInit(); 

		/* scan scene's materials for textures */
		for (unsigned int m=0; m<scene->mNumMaterials; ++m)
		{
			int texIndex = 0;
			aiString path;	// filename

			aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			while (texFound == AI_SUCCESS) {
				//fill map with textures, OpenGL image ids set to 0
				(*textureIdMap)[path.data] = 0; 
				// more textures?
				texIndex++;
				texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			}
		}

		int numTextures = textureIdMap->size();

		/* create and fill array with DevIL texture ids */
		ILuint* imageIds = new ILuint[numTextures];
		ilGenImages(numTextures, imageIds); 

		/* create and fill array with GL texture ids */
		GLuint* textureIds = new GLuint[numTextures];
		glGenTextures(numTextures, textureIds); /* Texture name generation */

		/* get iterator */
		std::map<std::string, GLuint>::iterator itr = textureIdMap->begin();

		for (int i=0; itr != textureIdMap->end(); ++i, ++itr)
		{
			//save IL image ID
			std::string filename = (*itr).first;  // get filename
			(*itr).second = textureIds[i];	  // save texture id for filename in map

			ilBindImage(imageIds[i]); /* Binding of DevIL image name */
			ilEnable(IL_ORIGIN_SET);
			ilOriginFunc(IL_ORIGIN_LOWER_LEFT); 
			std::string path = "../14db49e526f340dfba81c4a2da23c716/";
			//std::string path = "../c7130424fe904e71b69f4c5d02c55db3/";
			success = ilLoadImage((ILstring)(path+filename).c_str());

			if (success) {
				/* Convert image to RGBA */
				ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 

				/* Create and load textures to OpenGL */
				glBindTexture(GL_TEXTURE_2D, textureIds[i]); 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
					ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					ilGetData()); 
			}
			else 
				printf("Couldn't load Image: %s\n", filename.c_str());
		}
		/* Because we have already copied image data into texture data
		we can release memory used by image. */
		ilDeleteImages(numTextures, imageIds); 

		//Cleanup
		delete [] imageIds;
		delete [] textureIds;

		//return success;
		return true;
	}



	void printProgramInfoLog(GLuint obj)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;

		glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	    if (infologLength > 0)
	    {
	        infoLog = (char *)malloc(infologLength);
	        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
	        free(infoLog);
	    }
	}


	void changeSize(int w, int h)
	 {

		float ratio;
		// Prevent a divide by zero, when window is too short
		// (you cant make a window of zero width).
		if(h == 0)
			h = 1;

		// Set the viewport to be the entire window
	    glViewport(0, 0, w, h);

		ratio = (1.0f * w) / h;
		buildProjectionMatrix(53.13f, ratio, 0.1f, 100.0f);
	}

	#define printOpenGLError() printOglError(__FILE__, __LINE__)
	int printOglError(char *file, int line)
	{
	    
	    GLenum glErr;
	    int retCode = 0;

	    glErr = glGetError();
	    if (glErr != GL_NO_ERROR)
	    {
	        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
	        retCode = 1;
	    }
	    return retCode;
	}

	void processKeys(unsigned char key, int xx, int yy) 
	{
		double f = .05;
		switch(key)
		{
			case 27: glutLeaveMainLoop();break;
			case 'z': r -= 0.1f; break;
			case 'x': r += 0.1f; break;	
			case 'm': glEnable(GL_MULTISAMPLE); break;
			case 'n': glDisable(GL_MULTISAMPLE); break;
		}
		//camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
		//camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
		//camY = r *   						     sin(beta * 3.14f / 180.0f);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
	}


	void processMouseButtons(int button, int state, int xx, int yy) 
	{
		// // start tracking the mouse
		// if (state == GLUT_DOWN)  {
		// 	startX = xx;
		// 	startY = yy;
		// 	if (button == GLUT_LEFT_BUTTON)
		// 		tracking = 1;
		// 	else if (button == GLUT_RIGHT_BUTTON)
		// 		tracking = 2;
		// }

		// //stop tracking the mouse
		// else if (state == GLUT_UP) {
		// 	if (tracking == 1) {
		// 		alpha += (startX - xx);
		// 		beta += (yy - startY);
		// 	}
		// 	else if (tracking == 2) {
		// 		r += (yy - startY) * 0.01f;
		// 	}
		// 	tracking = 0;
		// }

	}


} // end namespace IO
