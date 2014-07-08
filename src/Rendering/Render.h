#ifndef RENDER_H_
#define RENDER_H_

#include "../Helper/MathHelp.h"
#include "../Helper/HelperStructures.h"
#include "../View/View.h"
#include "../IO/ProgramIO.h"


namespace Render 
{

    void timer(int);

    void popMatrix();

    void pushMatrix();

    void genVAOsAndUniformBuffer();

    void recursive_render (const aiNode* nd);

    void renderScene(void);

    void clearMeshes();

    void setModelMatrix();

    // The equivalent to glTranslate applied to the model matrix
    void translate(float x, float y, float z);

    // The equivalent to glRotate applied to the model matrix
    void rotate(float angle, float x, float y, float z);

    // The equivalent to glScale applied to the model matrix
    void scale(float x, float y, float z);

}

#endif