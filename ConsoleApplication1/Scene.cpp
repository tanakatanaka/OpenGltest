#include "stdafx.h"
#include "scene.h"
#include <glew.h>

class SceneBasic : public Scene
{
private:
    int width, height;
    GLuint vboHandles[2];
    GLuint vaoHandle;

    void linkMe(GLint vertShader, GLint fragShader);

public:
    SceneBasic();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
