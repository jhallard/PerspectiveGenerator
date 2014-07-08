#include "MathHelp.h"

GLuint matricesUniLoc = 1;
GLuint materialUniLoc = 2;

GLuint matricesUniBuffer = 0;

 // Program and Shader Identifiers
GLuint program = 0;
GLuint vertexShader = 0;
GLuint fragmentShader = 0;

// Vertex Attribute Locations
GLuint vertexLoc=0;
GLuint normalLoc=1;
GLuint texCoordLoc=2;

GLuint texUnit = 0;

// the global Assimp scene object
const aiScene* scene = NULL;

// scale factor for the model to fit in the window
float scaleFactor = 1;

std::string modelname;
std::string path;

// Shader Names
char *vertexfile = "../src/Shaders/Shader.vert";
char *fragmentfile = "../src/Shaders/Shader.frag";

std::vector<struct Helper::MyMesh> myMeshes;

// For push and pop matrix
std::vector<float *> matrixStack;

// Camera Position
float camera[3] = {0, 0, 0};

// mesh tranlation
float translation[3] = {0,  0,  0};

// Model Matrix (part of the OpenGL Model View Matrix)
float modelMatrix[16];

// Frame counting and FPS computation
long time1 = 0;
long timebase = 0;
long frame = 0;

char s[32];

std::vector<std::vector<float> > perspectiveList;
int currentPerspective = 0;
int perspectiveCount = 0;


std::map<std::string, GLuint> textureIdMap;

namespace MathHelp
{
    float DegToRad(float degrees) 
    { 
        return (float)(degrees * (M_PI / 180.0f));
    }


    // res = a cross b;
    void crossProduct( float *a, float *b, float *res) {

        res[0] = a[1] * b[2]  -  b[1] * a[2];
        res[1] = a[2] * b[0]  -  b[2] * a[0];
        res[2] = a[0] * b[1]  -  b[0] * a[1];
    }


    // Normalize a vec3
    void normalize(float *a) {

        float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);

        a[0] /= mag;
        a[1] /= mag;
        a[2] /= mag;
    }


    // sets the square matrix mat to the identity matrix,
    // size refers to the number of rows (or columns)
    void setIdentityMatrix( float *mat, int size) {

        // fill matrix with 0s
        for (int i = 0; i < size * size; ++i)
                mat[i] = 0.0f;

        // fill diagonal with 1s
        for (int i = 0; i < size; ++i)
            mat[i + i * size] = 1.0f;
    }


    //
    // a = a * b;
    //
    void multMatrix(float *a, float *b) {

        float res[16];

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[j*4 + i] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    res[j*4 + i] += a[k*4 + i] * b[j*4 + k]; 
                }
            }
        }
        memcpy(a, res, 16 * sizeof(float));

    }


    // Defines a transformation matrix mat with a translation
    void setTranslationMatrix(float *mat, float x, float y, float z) {

        setIdentityMatrix(mat,4);
        mat[12] = x;
        mat[13] = y;
        mat[14] = z;
    }

    // Defines a transformation matrix mat with a scale
    void setScaleMatrix(float *mat, float sx, float sy, float sz) {

        setIdentityMatrix(mat,4);
        mat[0] = sx;
        mat[5] = sy;
        mat[10] = sz;
    }

    // Defines a transformation matrix mat with a rotation 
    // angle alpha and a rotation axis (x,y,z)
    void setRotationMatrix(float *mat, float angle, float x, float y, float z) {

        float radAngle = DegToRad(angle);
        float co = cos(radAngle);
        float si = sin(radAngle);
        float x2 = x*x;
        float y2 = y*y;
        float z2 = z*z;

        mat[0] = x2 + (y2 + z2) * co; 
        mat[4] = x * y * (1 - co) - z * si;
        mat[8] = x * z * (1 - co) + y * si;
        mat[12]= 0.0f;
           
        mat[1] = x * y * (1 - co) + z * si;
        mat[5] = y2 + (x2 + z2) * co;
        mat[9] = y * z * (1 - co) - x * si;
        mat[13]= 0.0f;
           
        mat[2] = x * z * (1 - co) - y * si;
        mat[6] = y * z * (1 - co) + x * si;
        mat[10]= z2 + (x2 + y2) * co;
        mat[14]= 0.0f;
           
        mat[3] = 0.0f;
        mat[7] = 0.0f;
        mat[11]= 0.0f;
        mat[15]= 1.0f;
    }


    void set_float4(float f[4], float a, float b, float c, float d)
    {
        f[0] = a;
        f[1] = b;
        f[2] = c;
        f[3] = d;
    }

    void color4_to_float4(const aiColor4D *c, float f[4])
    {
        f[0] = c->r;
        f[1] = c->g;
        f[2] = c->b;
        f[3] = c->a;
    }

} // end nameapce MathHelp