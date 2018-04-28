#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneEdge : public Scene
{
private:
    Shader prog;

    int width, height;
    GLuint fsQuad, pass1Index, pass2Index, fboHandle;

    VBOPlane *plane;
    VBOTorus *torus;
    VBOTeapot *teapot;
    VBOCube *cube;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();

public:
    SceneEdge();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};