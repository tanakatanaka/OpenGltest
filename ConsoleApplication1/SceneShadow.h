#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include <glm/glm.hpp>
#include "frustum.h"
#include <memory>
using glm::mat4;

class SceneShadow : public Scene
{
private:
    Shader prog;
	Shader solidProg;

    int width, height;
	int shadowMapWidth, shadowMapHeight;
    GLuint shadowFBO, fsQuad, pass1Index, pass2Index, fboHandle;

	std::unique_ptr<Frustum> lightFrustum;

    VBOPlane *plane;
    VBOTorus *torus;
    VBOTeapot *teapot;
    VBOCube *cube;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;
	mat4 lightPV;
	mat4 shadowBias;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();
    void pass1();
    void pass2();
	void spitOutDepthBuffer();
	void drawScene();

public:
	SceneShadow();
    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};