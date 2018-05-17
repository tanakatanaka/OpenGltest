#pragma once

#include "scene.h"
#include "shader.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

class SceneSky : public Scene
{
private:
    Shader prog;

    GLuint quad;
    int width, height;

    mat4 model, view, projection;
    vec3 lightPos;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void drawScene();

public:
    SceneSky();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
