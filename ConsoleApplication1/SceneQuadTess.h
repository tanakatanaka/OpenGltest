#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomeshadj.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneQuadTess : public Scene
{
private:
    Shader prog;

    GLuint vaoHandle;
    int width, height;

    mat4 model;
    mat4 view, viewport;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneQuadTess();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};