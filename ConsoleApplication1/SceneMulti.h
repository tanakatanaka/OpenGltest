#pragma once

#include "scene.h"
#include "shader.h"
#include "vbomesh.h"
#include "vboplane.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneMulti : public Scene
{
private:
    Shader shader_;

    int width, height;
    VBOPlane *plane;
    VBOMesh * mesh;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneMulti();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};