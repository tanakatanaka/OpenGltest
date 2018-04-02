#pragma once

#include "scene.h"
#include "vbotorus.h"
#include "shader.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneADS : public Scene
{
private:
	Shader shader_;

    int width, height;
    VBOTorus *torus;
    float angle;

    mat4 model;
    mat4 view;
    mat4 projection;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneADS();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};