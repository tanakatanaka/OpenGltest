#pragma once


#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vboteapot.h"
#include "vbotorus.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneToon : public Scene
{
private:
    Shader prog;

    int width, height;
    VBOPlane *plane;
    VBOTeapot *teapot;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneToon();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};