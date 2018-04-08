#include "stdafx.h"
#include "scenediscard.h"
#include <cstdio>
#include <cstdlib>
#include "glutils.h"
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneDiscard::SceneDiscard() : angle(0.0f)
{
}

void SceneDiscard::initScene()
{
    compileAndLinkShader();

    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(13, mat4(1.0f));

    view = glm::lookAt(vec3(0.0f,0.0f,20.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,-1.0f,0.0f));
    projection = mat4(1.0f);

    shader_.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    shader_.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    shader_.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    shader_.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    shader_.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    shader_.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
    shader_.setUniform("Material.Shininess", 100.0f);

}

void SceneDiscard::update( float t )
{
    //angle += 1.0f;
    //if( angle > 360.0 ) angle -= 360.0;
}

void SceneDiscard::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4 lightPos = vec4(0.0f,0.0f,0.0f,1.0f);
    shader_.setUniform("Light.Position", lightPos );

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0,-1.5,0.0));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
}

void SceneDiscard::setMatrices()
{
    mat4 mv = view * model;
    shader_.setUniform("ModelViewMatrix", mv);
    shader_.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    shader_.setUniform("MVP", projection * mv);
}

void SceneDiscard::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(50.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneDiscard::compileAndLinkShader()
{
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/discard.vert",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               shader_.Log().c_str());
        exit(1);
    }
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/discard.frag",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               shader_.Log().c_str());
        exit(1);
    }
    if( ! shader_.Link() )
    {
        printf("Shader shader_ram failed to link!\n%s", shader_.Log().c_str());
        exit(1);
    }

    if( ! shader_.Validate() )
    {
        printf("shader_ram failed to validate!\n%s", shader_.Log().c_str());
        exit(1);
    }

    shader_.Use();
}
