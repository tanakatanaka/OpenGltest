#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomeshadj.h"
#include "vbomesh.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneSilhouette : public Scene
{
private:
    Shader prog;

    int width, height;

	VBOTorus *torus;
	VBOMeshAdj *ogre;

    mat4 model;
    mat4 view, viewport;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void setupFBO();

public:
    SceneSilhouette();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
