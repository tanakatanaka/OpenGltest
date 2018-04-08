#pragma once

#include "scene.h"
#include "Shader.h"
#include "vboteapot.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneDiscard : public Scene
{
private:
    Shader shader_;

    int width, height;
    VBOTeapot *teapot;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDiscard();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};