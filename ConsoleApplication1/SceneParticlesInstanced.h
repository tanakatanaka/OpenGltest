#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneParticlesInstanced : public Scene
{
private:
    Shader prog;

    int width, height;

    GLuint particleArray;
    GLuint initVel, startTime;

    int nParticles;

    VBOPlane *plane;
    VBOTorus *torus;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;
    float time, deltaT;

    void setMatrices();
    void compileAndLinkShader();
    void initBuffers();
    float randFloat();

public:
    SceneParticlesInstanced();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};