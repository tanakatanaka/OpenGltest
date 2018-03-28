#pragma once
#include "scene.h"
#include <glm/glm.hpp>
using glm::mat4;

class SceneBox : public Scene
{
private:
    int width, height;
    GLuint vboHandles[2];
    GLuint vaoHandle;
	GLuint programHandle;
	float angle;
	mat4 rotationMatrix;


    void linkMe(GLint vertShader, GLint fragShader);
	void printActiveUniforms(GLuint);
	void InitUniformBlockBuffer();

public:
    SceneBox();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};
