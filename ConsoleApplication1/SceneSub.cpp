#include "stdafx.h"
#include "SceneSub.h"
#include <cstdio>
#include <cstdlib>

#include "glutils.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


SceneSub::SceneSub() : angle(0.0f)
{
}

void SceneSub::initScene()
{
    compileAndLinkShader();

    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(13, mat4(1.0f));
    torus = new VBOTorus(0.7f, 0.3f, 30, 30);

    view = glm::lookAt(vec3(0.0f,0.0f,10.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    shader_.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    shader_.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    shader_.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    shader_.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    shader_.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    shader_.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
    shader_.setUniform("Material.Shininess", 100.0f);

}

void SceneSub::update( float t )
{
    //angle += 1.0f;
    //if( angle > 360.0 ) angle -= 360.0;
}

void SceneSub::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec4 lightPos = vec4(0.0f,0.0f,0.0f,1.0f);
    shader_.setUniform("Light.Position", lightPos );

    GLuint shader_ramHandle = shader_.GetHandle();

    GLuint adsIndex = glGetSubroutineIndex( shader_ramHandle, GL_VERTEX_SHADER, "phongModel" );
    GLuint diffuseIndex = glGetSubroutineIndex(shader_ramHandle, GL_VERTEX_SHADER, "diffuseOnly");

    glUniformSubroutinesuiv( GL_VERTEX_SHADER, 1, &adsIndex);
    model = mat4(1.0f);
    model *= glm::translate(vec3(-3.0,-1.5,0.0));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    glUniformSubroutinesuiv( GL_VERTEX_SHADER, 1, &diffuseIndex);
    model = mat4(1.0f);
    model *= glm::translate(vec3(3.0f, -1.5f, 0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
}

void SceneSub::setMatrices()
{
    mat4 mv = view * model;
    shader_.setUniform("ModelViewMatrix", mv);
    shader_.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    shader_.setUniform("MVP", projection * mv);
}

void SceneSub::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(50.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneSub::compileAndLinkShader()
{
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/subroutine.vert",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s", shader_.Log().c_str());
        exit(1);
    }
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/subroutine.frag",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s", shader_.Log().c_str());
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
