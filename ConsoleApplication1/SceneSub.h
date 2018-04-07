#pragma once

#include "stdafx.h"

#include "scene.h"
#include "shader.h"
#include "vboteapot.h"
#include "vbotorus.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneSub : public Scene
{
private:
    Shader shader_;

    int width, height;
    VBOTorus *torus;
    VBOTeapot *teapot;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneSub();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};