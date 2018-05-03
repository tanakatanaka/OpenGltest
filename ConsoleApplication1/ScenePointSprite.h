#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomesh.h"
#include <glm/glm.hpp>
using glm::mat4;

class ScenePointSprite : public Scene
{
private:
    Shader prog;

    int width, height;
    GLuint sprites;

    VBOPlane *plane;
    VBOTorus *torus;
    VBOMesh *ogre;
    VBOCube *cube;

    int numSprites;
    float *locations;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();

public:
    ScenePointSprite();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
