#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"

#include <glm/glm.hpp>
using glm::mat4;

class SceneFire : public Scene
{
private:
    Shader prog;

    int width, height;

    GLuint posBuf[2], velBuf[2];
    GLuint particleArray[2];
    GLuint feedback[2], initVel, startTime[2];
    GLuint drawBuf, query;
    GLuint renderSub, updateSub;

    int nParticles;

    VBOPlane *plane;

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
    SceneFire();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
