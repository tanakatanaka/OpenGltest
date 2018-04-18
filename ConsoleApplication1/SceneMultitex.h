#include "scene.h"
#include "shader.h"
#include "vboplane.h"
#include "vbocube.h"
#include "defines.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneMultiTex : public Scene
{
private:
    Shader prog;

    int width, height;
    VBOPlane *plane;
    VBOCube *cube;

    mat4 model;
    mat4 view;
    mat4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneMultiTex();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
