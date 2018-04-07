#pragma once

#include "scene.h"
#include "shader.h"
#include "vbomesh.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneFlat : public Scene
{
private:
    Shader shader_;

    int width, height;
    VBOMesh *ogre;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneFlat();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};