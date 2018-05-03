#pragma once
#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomesh.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneDeferred : public Scene
{
private:
    Shader prog;

    int width, height;
    GLuint deferredFBO;
    GLuint quad, pass1Index, pass2Index;

    VBOPlane *plane;
    VBOTorus *torus;
    VBOMesh *ogre;
    VBOCube *cube;
    VBOTeapot *teapot;

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
    SceneDeferred();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};