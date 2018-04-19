#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbomesh.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneNormalMap : public Scene
{
private:
    Shader prog;

    int width, height;
    VBOPlane *plane;
    VBOCube *cube;
    VBOMesh *ogre;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneNormalMap();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};