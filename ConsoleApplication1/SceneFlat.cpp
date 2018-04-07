#include "stdafx.h"
#include "sceneflat.h"
#include <cstdio>
#include <cstdlib>
#include "glutils.h"

using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneFlat::SceneFlat() : angle(0.0f)
{
}

void SceneFlat::initScene()
{
    compileAndLinkShader();

    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);

    ogre = new VBOMesh("../ConsoleApplication1/Media/bs_ears.obj");

    view = glm::lookAt(vec3(0.0f,0.35f,0.85f), vec3(0.0f,-0.25f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    shader_.setUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
    shader_.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    shader_.setUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
    shader_.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    shader_.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    shader_.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
    shader_.setUniform("Material.Shininess", 100.0f);
}

void SceneFlat::update( float t )
{
    //angle += 1.0f;
    //if( angle > 360.0 ) angle -= 360.0;
}

void SceneFlat::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    vec4 worldLight = vec4(2.0f,4.0f,1.0f,1.0f);
    model = glm::rotate(angle, vec3(0.0f,1.0f,0.0f));
    shader_.setUniform("Light.Position", view * model * worldLight );

    model = mat4(1.0f);
    //model *= glm::translate(vec3(0.0,-1.0,0.0));
    model *= glm::rotate(25.0f, vec3(0.0f,1.0f,0.0f));
    //model *= glm::rotate(140.0f, vec3(0.0f,1.0f,0.0f));

    setMatrices();
    ogre->render();
}

void SceneFlat::setMatrices()
{
    mat4 mv = view * model;
    shader_.setUniform("ModelViewMatrix", mv);
    shader_.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    shader_.setUniform("MVP", projection * mv);
}

void SceneFlat::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(70.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneFlat::compileAndLinkShader()
{
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/phong.vert",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s", shader_.Log().c_str());
        exit(1);
    }
    if( ! shader_.CompileShaderFromFile("../ConsoleApplication1/Shader/phong.frag",GLSLShader::FRAGMENT))
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
