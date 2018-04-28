#pragma once

#include "scene.h"
#include "Shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboteapot.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneRenderToTex : public Scene
{
private:
    Shader prog;

    GLuint fboHandle;
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
    void setupFBO();
    void renderToTexture();
    void renderScene();

public:
    SceneRenderToTex();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};