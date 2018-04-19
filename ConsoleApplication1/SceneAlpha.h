#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboteapot.h"


#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
using glm::mat4;

class SceneAlpha : public Scene
{
private:
    Shader prog;

    int width, height;
    VBOPlane *plane;
    VBOCube *cube;
    VBOTeapot *teapot;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneAlpha();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};