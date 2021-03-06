#include "stdafx.h"
#include "scenediffuse.h"
#include <cstdio>
#include <cstdlib>
#include "Glutils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
using glm::vec3;
using glm::vec4;



SceneDiffuse::SceneDiffuse()
{
}

void SceneDiffuse::initScene()
{
    compileAndLinkShader();
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);

    torus = new VBOTorus(0.7f, 0.3f, 30, 30);

    model = mat4(1.0f);
    model *= glm::rotate(-35.0f, vec3(1.0f,0.0f,0.0f));
    model *= glm::rotate(35.0f, vec3(0.0f,1.0f,0.0f));
    view = glm::lookAt(vec3(0.0f,0.0f,2.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

	shader_.setUniform("Kd", 0.9f, 0.5f, 0.3f);
	shader_.setUniform("Ld", 1.0f, 1.0f, 1.0f);
	shader_.setUniform("LightPosition", view * vec4(5.0f, 5.0f, 2.0f, 1.0f));
}


void SceneDiffuse::update( float t )
{

}

void SceneDiffuse::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setMatrices();
    torus->render();
}

void SceneDiffuse::setMatrices()
{
    mat4 mv = view * model;
	shader_.setUniform("ModelViewMatrix", mv);
	shader_.setUniform("NormalMatrix", mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
	shader_.setUniform("MVP", projection * mv);
}

void SceneDiffuse::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(70.0f, (float)w / h, 0.3f, 100.0f);
}
void SceneDiffuse::compileAndLinkShader()
{
	if (!shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/diffuse.vert", GLSLShader::VERTEX))
	{
		printf("Fragment shader failed to compile!\n%s", shader_.Log().c_str());
		exit(1);
	}
	if (!shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/diffuse.frag", GLSLShader::FRAGMENT))
	{
		printf("Fragment shader failed to compile!\n%s", shader_.Log().c_str());
		exit(1);
	}

	if (!shader_.Link())
	{
		printf("Shader program failed to link!\n%s", shader_.Log().c_str());
		exit(1);
	}
	shader_.Use();
}
