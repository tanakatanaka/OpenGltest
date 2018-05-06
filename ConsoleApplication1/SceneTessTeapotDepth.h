#pragma once

#include "scene.h"
#include "shader.h"
#include "vboteapotpatch.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneTessTeapotDepth : public Scene
{
private:
    Shader prog;

    GLuint vaoHandle;
    int width, height;

    VBOTeapotPatch *teapot;

    mat4 model;
    mat4 view, viewport;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTessTeapotDepth();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
