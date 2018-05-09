#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "frustum.h"
#include <memory>

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

class SceneShadowMap : public Scene
{
private:
    Shader prog, solidProg;
    GLuint shadowFBO, pass1Index, pass2Index;

    std::unique_ptr<VBOTeapot> teapot;
	std::unique_ptr<VBOPlane> plane;
	std::unique_ptr<VBOTorus> torus;

    int width, height;
    int shadowMapWidth, shadowMapHeight;

    mat4 model, view, projection;
    mat4 lightPV;
    mat4 shadowBias;
    float angle;

	std::unique_ptr<Frustum> lightFrustum;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void drawScene();
    void spitOutDepthBuffer();

public:
    SceneShadowMap();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};