#pragma once


#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbomesh.h"
#include "vboteapot.h"
#include "vbotorus.h"
#include "skybox.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneProjTex : public Scene
{
private:
    Shader prog;

    int width, height;
    VBOTeapot *teapot;
    VBOTorus *torus;
    VBOPlane *plane;
    SkyBox *sky;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneProjTex();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};