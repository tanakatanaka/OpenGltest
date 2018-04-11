#include "stdafx.h"
#include "scenefragment.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneFragment::SceneFragment()
{
}

void SceneFragment::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    teapot = new VBOTeapot(14, glm::mat4(1.0f));
    torus = new VBOTorus(0.7f * 2, 0.3f * 2, 50, 50);

    view = glm::lookAt(vec3(2.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f), vec3(0.0f,-1.0f,0.0f));
    projection = mat4(1.0f);
    angle = 0.957283;
	//angle = 15.957283;
    shader_.setUniform("LightIntensity", vec3(0.9f,0.9f,0.9f) );
}

void SceneFragment::update( float t )
{
    angle += 0.01f;
    if( angle > TWOPI) angle -= TWOPI;
}

void SceneFragment::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_.setUniform("LightPosition", view * vec4(10.0f * cos(angle), 3.0f, 10.0f * sin(angle), 1.0f) );
	//printf("%f\n", angle);
    shader_.setUniform("Kd", 0.9f, 0.5f, 0.3f);
    shader_.setUniform("Ks", 0.95f, 0.95f, 0.95f);
    shader_.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    shader_.setUniform("Shininess", 100.0f);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,0.0f,0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    shader_.setUniform("Kd", 0.7f, 0.7f, 0.7f);
    shader_.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    shader_.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    shader_.setUniform("Shininess", 180.0f);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,-0.45f,0.0f));
    setMatrices();
    plane->render();
}

void SceneFragment::setMatrices()
{
    mat4 mv = view * model;
    shader_.setUniform("ModelViewMatrix", mv);
    shader_.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    shader_.setUniform("MVP", projection * mv);
}

void SceneFragment::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(60.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneFragment::compileAndLinkShader()
{
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/perfrag.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               shader_.Log().c_str());
        exit(1);
    }
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/perfrag.fs",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               shader_.Log().c_str());
        exit(1);
    }
    if( ! shader_.Link() )
    {
        printf("Shader shader_ram failed to link!\n%s",
               shader_.Log().c_str());
        exit(1);
    }

    shader_.Use();
}