#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomesh.h"
#include "frustum.h"

#include <memory>
#include <glm/glm.hpp>

#include <cstdio>

#include "glutils.h"
#include "defines.h"


using glm::mat4;
using glm::vec4;
using glm::vec3;


class ScenePcf : public Scene
{
private:
    Shader prog;
    GLuint shadowFBO, pass1Index, pass2Index;

    std::unique_ptr<VBOTeapot> teapot;
    std::unique_ptr<VBOPlane> plane;
    std::unique_ptr<VBOTorus> torus;
    std::unique_ptr<VBOMesh> mesh;

    Frustum *lightFrustum;

    int width, height;
    int shadowMapWidth, shadowMapHeight;

    mat4 model, view, projection;
    mat4 lightView, lightProjection, lightPV;
    mat4 shadowScale;
    vec3 lightPos;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void drawScene();
    void drawBuildingScene();

public:
    ScenePcf();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};