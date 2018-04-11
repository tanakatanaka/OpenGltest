#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vboteapot.h"
#include "vbotorus.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneFragment : public Scene
{
private:
    Shader shader_;

    int width, height;
    VBOPlane *plane;
    VBOTeapot *teapot;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneFragment();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
