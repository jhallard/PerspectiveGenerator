#include "View.h"

namespace View
{

    void buildProjectionMatrix(float fov, float ratio, float nearp, float farp)
     {

        float projMatrix[16];

        float f = 1.0f / tan (fov * (M_PI / 360.0f));

        MathHelp::setIdentityMatrix(projMatrix,4);

        projMatrix[0] = f / ratio;
        projMatrix[1 * 4 + 1] = f;
        projMatrix[2 * 4 + 2] = (farp + nearp) / (nearp - farp);
        projMatrix[3 * 4 + 2] = (2.0f * farp * nearp) / (nearp - farp);
        projMatrix[2 * 4 + 3] = -1.0f;
        projMatrix[3 * 4 + 3] = 0.0f;

        glBindBuffer(GL_UNIFORM_BUFFER,matricesUniBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, ProjMatrixOffset, MatrixSize, projMatrix);
        glBindBuffer(GL_UNIFORM_BUFFER,0);

    }

    void setCamera(float posX, float posY, float posZ, float lookAtX, float lookAtY, float lookAtZ)
    {

        float dir[3], right[3], up[3];

        up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;

        dir[0] =  (lookAtX - posX);
        dir[1] =  (lookAtY - posY);
        dir[2] =  (lookAtZ - posZ);
        MathHelp::normalize(dir);

        MathHelp::crossProduct(dir,up,right);
        MathHelp::normalize(right);

        MathHelp::crossProduct(right,dir,up);
        MathHelp::normalize(up);

        float viewMatrix[16],aux[16];

        viewMatrix[0]  = right[0];
        viewMatrix[4]  = right[1];
        viewMatrix[8]  = right[2];
        viewMatrix[12] = 0.0f;

        viewMatrix[1]  = up[0];
        viewMatrix[5]  = up[1];
        viewMatrix[9]  = up[2];
        viewMatrix[13] = 0.0f;

        viewMatrix[2]  = -dir[0];
        viewMatrix[6]  = -dir[1];
        viewMatrix[10] = -dir[2];
        viewMatrix[14] =  0.0f;

        viewMatrix[3]  = 0.0f;
        viewMatrix[7]  = 0.0f;
        viewMatrix[11] = 0.0f;
        viewMatrix[15] = 1.0f;

        MathHelp::setTranslationMatrix(aux, -posX, -posY, -posZ);

        MathHelp::multMatrix(viewMatrix, aux);
        
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, ViewMatrixOffset, MatrixSize, viewMatrix);
        glBindBuffer(GL_UNIFORM_BUFFER,0);
    }


    void get_bounding_box_for_node (const aiNode* nd, aiVector3D* min, aiVector3D* max)
    {
        aiMatrix4x4 prev;
        unsigned int n = 0, t;

        for (; n < nd->mNumMeshes; ++n)
        {
            const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
            for (t = 0; t < mesh->mNumVertices; ++t) 
            {

                aiVector3D tmp = mesh->mVertices[t];

                min->x = aisgl_min(min->x,tmp.x);
                min->y = aisgl_min(min->y,tmp.y);
                min->z = aisgl_min(min->z,tmp.z);

                max->x = aisgl_max(max->x,tmp.x);
                max->y = aisgl_max(max->y,tmp.y);
                max->z = aisgl_max(max->z,tmp.z);
            }
        }

        for (n = 0; n < nd->mNumChildren; ++n)
        {
            get_bounding_box_for_node(nd->mChildren[n],min,max);
        }
    }


    void get_bounding_box (aiVector3D* min, aiVector3D* max)
    {

        min->x = min->y = min->z =  1e10f;
        max->x = max->y = max->z = -1e10f;
        get_bounding_box_for_node(scene->mRootNode,min,max);
    }

} // end namespace View