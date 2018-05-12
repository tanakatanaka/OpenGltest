#include "stdafx.h"
#include "scenepointsprite.h"
#include <cstdio>
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;
#include <SDL_image.h>
#include "glutils.h"
#include "defines.h"
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


using std::unique_ptr;

ScenePointSprite::ScenePointSprite()
{
    width = 640;
    height = 480;
}

void ScenePointSprite::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    float c = 2.5f;
    //projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);

    angle = PI / 2.0;

    numSprites = 100;
    locations = new float[numSprites * 3];
    srand( time(0) );
    for( int i = 0; i < numSprites; i++ )
	{
        vec3 p(((float)rand() / RAND_MAX * 2.0f) - 1.0f,
               ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
               ((float)rand() / RAND_MAX * 2.0f) - 1.0f);
        locations[i*3] = p.x;
        locations[i*3+1] = p.y;
        locations[i*3+2] = p.z;
    }

    // Set up the buffers
    GLuint handle;
    glGenBuffers(1, &handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glBufferData(GL_ARRAY_BUFFER, numSprites * 3 * sizeof(float), locations, GL_STATIC_DRAW);

    delete [] locations;

    // Set up the vertex array object
    glGenVertexArrays( 1, &sprites );
    glBindVertexArray(sprites);

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
    glEnableVertexAttribArray(0);  // Vertex position

    glBindVertexArray(0);

    // Load texture file
    const char * texName = "../Texture/flower.png";
    //QImage spriteImg = QGLWidget::convertToGLFormat(QImage(texName,"PNG"));
	auto hoge = SDL_SurfacePtr(IMG_Load(texName));
	auto spriteImg = convert_to_opengl_format(hoge.get());

    // Copy texture to OpenGL
    GLuint texID;
    glGenTextures(1, &texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spriteImg->w, spriteImg->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, spriteImg->pixels);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    prog.setUniform("SpriteTex", 0);
    prog.setUniform("Size2", 0.15f);
}


void ScenePointSprite::update( float t )
{
    angle += 0.001f;
    if( angle > TWOPI) angle -= TWOPI;
}

void ScenePointSprite::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos(3.0f * cos(angle),0.0f,3.0f * sin(angle));
    view = glm::lookAt(cameraPos,
                       vec3(0.0f,0.0f,0.0f),
                       vec3(0.0f,1.0f,0.0f));

    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(sprites);
    glDrawArrays(GL_POINTS, 0, numSprites);

    glFinish();
}

void ScenePointSprite::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("ProjectionMatrix", projection);
}

void ScenePointSprite::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(200.0f, (float)w/h, 0.3f, 1000.0f);
}

void ScenePointSprite::compileAndLinkShader()
{
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/pointsprite.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.Log().c_str());
        exit(1);
    }
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/pointsprite.gs",GLSLShader::GEOMETRY) )
    {
        printf("Geometry shader failed to compile!\n%s",
               prog.Log().c_str());
        exit(1);
    }
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/pointsprite.fs",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               prog.Log().c_str());
        exit(1);
    }
    if( ! prog.Link() )
    {
        printf("Shader program failed to link!\n%s",
               prog.Log().c_str());
        exit(1);
    }

    prog.Use();
}
