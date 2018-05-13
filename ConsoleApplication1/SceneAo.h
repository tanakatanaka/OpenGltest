#pragma once

#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"
#include "vbomesh.h"
#include "frustum.h"

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

class SceneAo : public Scene
{
private:
    Shader prog;

    VBOTeapot *teapot;
    VBOPlane *plane;
    VBOTorus *torus;
    VBOMesh *ogre;

    int width, height;

    mat4 model, view, projection;
    vec3 lightPos;
    float angle;

    void setMatrices();
    void compileAndLinkShader();
    void drawScene();

public:
    SceneAo();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
