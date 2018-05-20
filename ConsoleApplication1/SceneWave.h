#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneWave : public Scene
{
private:
    Shader prog;

    int width, height;

    VBOPlane *plane;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;
    float time;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneWave();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
