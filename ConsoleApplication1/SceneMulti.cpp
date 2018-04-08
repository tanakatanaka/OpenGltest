#include "stdafx.h"
#include "sceneMulti.h"

#include <cstdio>
#include <cstdlib>

#include "glutils.h"
#include "defines.h"

using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneMulti::SceneMulti()
{
}

void SceneMulti::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    plane = new VBOPlane(10.0f, 10.0f, 100, 100);
    mesh = new VBOMesh("../ConsoleApplication1/Media/pig_triangulated.obj");

   // model *= glm::rotate(35.0f, vec3(0.0f,1.0f,0.0f));
    view = glm::lookAt(vec3(0.5f,0.75f,0.75f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    char name[20];
    float x, z;
    for( int i = 0; i < 5; i++ )
	{
        snprintf(name, 20, "lights[%d].Position", i);
        x = 2.0 * cos((TWOPI / 5) * i);
        z = 2.0 * sin((TWOPI / 5) * i);
        shader_.setUniform(name, view * vec4(x, 1.2f, z + 1.0f, 1.0f) );
    }

    shader_.setUniform("lights[0].Intensity", vec3(0.0f,0.8f,0.8f) );
    shader_.setUniform("lights[1].Intensity", vec3(0.0f,0.0f,0.8f) );
    shader_.setUniform("lights[2].Intensity", vec3(0.8f,0.0f,0.0f) );
    shader_.setUniform("lights[3].Intensity", vec3(0.0f,0.8f,0.0f) );
    shader_.setUniform("lights[4].Intensity", vec3(0.8f,0.8f,0.8f) );
}

void SceneMulti::update( float t )
{

}

void SceneMulti::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_.setUniform("Kd", 0.4f, 0.4f, 0.4f);
    shader_.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    shader_.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    shader_.setUniform("Shininess", 180.0f);

    model = mat4(1.0f);
    model *= glm::rotate(90.0f, vec3(0.0f,1.0f,0.0f));
    setMatrices();
    mesh->render();

    shader_.setUniform("Kd", 0.1f, 0.1f, 0.1f);
    shader_.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    shader_.setUniform("Ka", 0.1f, 0.1f, 0.1f);
    shader_.setUniform("Shininess", 180.0f);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,-0.45f,0.0f));
    setMatrices();
    plane->render();
}

void SceneMulti::setMatrices()
{
    mat4 mv = view * model;
    shader_.setUniform("ModelViewMatrix", mv);
    shader_.setUniform("NormalMatrix", mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    shader_.setUniform("MVP", projection * mv);
}

void SceneMulti::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(70.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneMulti::compileAndLinkShader()
{
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/multilight.vert",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s", shader_.Log().c_str());
        exit(1);
    }
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/multilight.frag",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s", shader_.Log().c_str());
        exit(1);
    }
    if( ! shader_.Link() )
    {
        printf("Shader shader_ram failed to link!\n%s", shader_.Log().c_str());
        exit(1);
    }

    shader_.Use();
}
