#include "Render.h"

namespace Render
{

    void pushMatrix()
    {
        float *aux = (float *)malloc(sizeof(float) * 16);
        memcpy(aux, modelMatrix, sizeof(float) * 16);
        matrixStack.push_back(aux);
    }

    void popMatrix() 
    {
        float *m = matrixStack[matrixStack.size()-1];
        memcpy(modelMatrix, m, sizeof(float) * 16);
        matrixStack.pop_back();
        free(m);
    }



    void genVAOsAndUniformBuffer() 
    {

        struct Helper::MyMesh aMesh;
        struct Helper::MyMaterial aMat; 
        GLuint buffer;
        
        // For each mesh
        for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
        {
            const aiMesh* mesh = scene->mMeshes[n];

            // create array with faces
            // have to convert from Assimp format to array
            unsigned int *faceArray;
            faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
            unsigned int faceIndex = 0;

            for (unsigned int t = 0; t < mesh->mNumFaces; ++t) 
            {
                const aiFace* face = &mesh->mFaces[t];

                memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
                faceIndex += 3;
            }

            aMesh.numFaces = scene->mMeshes[n]->mNumFaces;

            // generate Vertex Array for mesh
            glGenVertexArrays(1,&(aMesh.vao));
            glBindVertexArray(aMesh.vao);

            // buffer for faces
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

            // buffer for vertex positions
            if (mesh->HasPositions()) 
            {
                glGenBuffers(1, &buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(vertexLoc);
                glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
            }

            // buffer for vertex normals
            if (mesh->HasNormals()) 
            {
                glGenBuffers(1, &buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
                glEnableVertexAttribArray(normalLoc);
                glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
            }

            // buffer for vertex texture coordinates
            if (mesh->HasTextureCoords(0)) {
                float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
                for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

                    texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
                    texCoords[k*2+1] = mesh->mTextureCoords[0][k].y; 
                    
                }
                glGenBuffers(1, &buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
                glEnableVertexAttribArray(texCoordLoc);
                glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
            }

            // unbind buffers
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        
            // create material uniform buffer
            aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
                
            aiString texPath;   //contains filename of texture
            if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath))
            {
                //bind texture
                unsigned int texId = (textureIdMap)[texPath.data];
                aMesh.texIndex = texId;
                aMat.texCount = 1;
            }
            else
            {
                std::cout << "AI_SUCCESS = False for mtl->getTexture Render.cpp" << std::endl;
                aMat.texCount = 0;
            }

            float c[4];
            MathHelp::set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
            aiColor4D diffuse;
            if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
                MathHelp::color4_to_float4(&diffuse, c);
            memcpy(aMat.diffuse, c, sizeof(c));

            MathHelp::set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
            aiColor4D ambient;
            if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
                MathHelp::color4_to_float4(&ambient, c);
            memcpy(aMat.ambient, c, sizeof(c));

            MathHelp::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
            aiColor4D specular;
            if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
                MathHelp::color4_to_float4(&specular, c);
            memcpy(aMat.specular, c, sizeof(c));

            MathHelp::set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
            aiColor4D emission;
            if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
                MathHelp::color4_to_float4(&emission, c);
            memcpy(aMat.emissive, c, sizeof(c));

            float shininess = 0.0;
            unsigned int max;
            aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
            aMat.shininess = shininess;

            glGenBuffers(1,&(aMesh.uniformBlockIndex));
            glBindBuffer(GL_UNIFORM_BUFFER,aMesh.uniformBlockIndex);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW);

            myMeshes.push_back(aMesh);
        }
    }




    void recursive_render(const aiNode* nd)
    {

        // Get node transformation matrix
        aiMatrix4x4 m = nd->mTransformation;
        // OpenGL matrices are column major
        m.Transpose();

        // save model matrix and apply node transformation
        pushMatrix();

        float aux[16];
        memcpy(aux,&m,sizeof(float) * 16);
        MathHelp::multMatrix(modelMatrix, aux);
        setModelMatrix();


        // draw all meshes assigned to this node
        for (unsigned int n=0; n < nd->mNumMeshes; ++n)
        {
            // bind material uniform
            glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[nd->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct Helper::MyMaterial)); 
            // bind texture
            glBindTexture(GL_TEXTURE_2D, myMeshes[nd->mMeshes[n]].texIndex);
            // bind VAO
            glBindVertexArray(myMeshes[nd->mMeshes[n]].vao);
            // draw
            glDrawElements(GL_TRIANGLES,myMeshes[nd->mMeshes[n]].numFaces*3,GL_UNSIGNED_INT,0);
        }

        // draw all children
        for (unsigned int n=0; n < nd->mNumChildren; ++n)
        {
            recursive_render(nd->mChildren[n]);
        }
        popMatrix();
    }


    // Rendering Callback Function

    void renderScene(void)
    {
        //std::cout << "render" << std::endl;
        static float step = 0.0f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        View::setCamera(camera[0], camera[1], camera[2],translation[0], translation[1], translation[2]);

        step += .02;
        // set the model matrix to the identity Matrix
        MathHelp::setIdentityMatrix(modelMatrix,4);

        // sets the model matrix to a scale matrix so that the model fits in the window
        scale(scaleFactor, scaleFactor, scaleFactor);

        // keep rotating the model
        //rotate(step, 0.0f, 1.0f, 0.0f);
        
        // use our shader
        glUseProgram(program);

        // we are only going to use texture unit 0
        // unfortunately samplers can't reside in uniform blocks
        // so we have set this uniform separately
        glUniform1i(texUnit,0);

        recursive_render(scene->mRootNode);

        // FPS computation and display
        frame++;
        time1=glutGet(GLUT_ELAPSED_TIME);
        if (time1 - timebase > 1000) 
        {
            sprintf(s,"FPS:%4.2f", frame*1000.0/(time1-timebase));
            timebase = time1;
            frame = 0;
            glutSetWindowTitle(s);
        }

        // swap buffers
        glutSwapBuffers();


        if(start)
        {
            unsigned char * pixels = new unsigned char[width * height * 3 ];
            glReadPixels( 0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels );
            cv::Mat image(height, width, CV_8UC3);
            int index = 0;
            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    cv::Point3_<uchar> * p = image.ptr<cv::Point3_<uchar> >(height-i-1, j);
                    p->x = pixels[index++];
                    p->y = pixels[index++];
                    p->z = pixels[index++];
                }
            }
            std::stringstream ss;
            ss << "../GeneratedImages/picture" << imageNum << ".png";
            std::string z = ss.str();
            cv::imwrite(z, image);
            imageNum++;
            delete [] pixels;

            IO::nextLocation(1);

            if(imageNum == perspectiveCount)
            {
                imageNum = 0;
                start = false;
            }
        }

        // increase the rotation angle
        //step++;
    }


    void clearMeshes()
    {
            // clear myMeshes stuff
        for (unsigned int i = 0; i < myMeshes.size(); ++i) {
                
            glDeleteVertexArrays(1,&(myMeshes[i].vao));
            glDeleteTextures(1,&(myMeshes[i].texIndex));
            glDeleteBuffers(1,&(myMeshes[i].uniformBlockIndex));
        }
    }

    void setModelMatrix() 
    {
        glBindBuffer(GL_UNIFORM_BUFFER,matricesUniBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, ModelMatrixOffset, MatrixSize, modelMatrix);
        glBindBuffer(GL_UNIFORM_BUFFER,0);
    }

    // The equivalent to glTranslate applied to the model matrix
    void translate(float x, float y, float z) 
    {
        float aux[16];
        MathHelp::setTranslationMatrix(aux,x,y,z);
        MathHelp::multMatrix(modelMatrix,aux);
        Render::setModelMatrix();
    }

    // The equivalent to glRotate applied to the model matrix
    void rotate(float angle, float x, float y, float z) 
    {
        float aux[16];
        MathHelp::setRotationMatrix(aux,angle,x,y,z);
        MathHelp::multMatrix(modelMatrix,aux);
        Render::setModelMatrix();
    }

    // The equivalent to glScale applied to the model matrix
    void scale(float x, float y, float z)
    {
        float aux[16];
        MathHelp::setScaleMatrix(aux,x,y,z);
        MathHelp::multMatrix(modelMatrix,aux);
        Render::setModelMatrix();
    }

} // end namespace Render