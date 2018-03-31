#pragma once
#include "vbotorus.h"
#include "defines.h"
#include "scene.h"
#include "shader.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneDiffuse : public Scene
{
private:
    Shader prog;

    int width, height;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDiffuse();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};