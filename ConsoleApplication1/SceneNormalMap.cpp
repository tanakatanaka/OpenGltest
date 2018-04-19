#include "stdafx.h"

#include "scenenormalmap.h"
#include <cstdio>
#include <cstdlib>
#include <SDL_image.h>
#include "glutils.h"
#include "defines.h"

using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


static SDL_Surface * convert_to_opengl_format(SDL_Surface *s)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    static const Uint32 r = 0xFF000000;
    static const Uint32 g = 0x00FF0000;
    static const Uint32 b = 0x0000FF00;
    static const Uint32 a = 0x000000FF;
#else
    static const Uint32 r = 0x000000FF;
    static const Uint32 g = 0x0000FF00;
    static const Uint32 b = 0x00FF0000;
    static const Uint32 a = 0xFF000000;
#endif
    
    //if (SDL_SetSurfaceBlendMode(s, SDL_BLENDMODE_NONE) { throw "failed to set alpha"; }
    
    int w = s->w;
    int h = s->h;
    
    SDL_Surface *converted;
    converted = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, r, g, b, a);
    
    if (SDL_BlitSurface(s, NULL, converted, NULL) != 0) { throw "failed to blit"; }

    return converted;
}

SceneNormalMap::SceneNormalMap()
{
}

void SceneNormalMap::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    //plane = new VBOPlane(50.0f, 50.0f, 1, 1);
    //cube = new VBOCube();
    ogre = new VBOMesh("../ConsoleApplication1/Media/bs_ears.obj");

    view = glm::lookAt(vec3(-1.0f,0.25f,2.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    angle = (float)( TO_RADIANS(100.0) );

    prog.setUniform("Light.Intensity", vec3(0.9f,0.9f,0.9f) );

    GLuint texIDs[2];
    glGenTextures(2, texIDs);

    // Load brick texture file
    const char * texName = "../Texture/ogre_diffuse.png";
    SDL_Surface *hoge = IMG_Load(texName);
    //QImage brickImg = QGLWidget::convertToGLFormat(QImage(texName,"JPG"));
	auto brickImg = convert_to_opengl_format(hoge);

    // Copy brick texture to OpenGL
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIDs[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, brickImg->w, brickImg->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, brickImg->pixels);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    prog.setUniform("ColorTex", 0);

    // Load moss texture file
    texName = "../Texture/ogre_normalmap.png";
	hoge = IMG_Load(texName);
    auto mossImg = convert_to_opengl_format(hoge);

    // Copy moss texture to OpenGL
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texIDs[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mossImg->w, mossImg->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, mossImg->pixels);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    prog.setUniform("NormalMapTex", 1);
}

void SceneNormalMap::update( float t )
{
    angle += 0.0001f;
    if( angle > TWOPI) angle -= TWOPI;
}

void SceneNormalMap::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Light.Position", view * vec4(10.0f * cos(angle),1.0f,10.0f * sin(angle),1.0f) );
    prog.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);
    setMatrices();
    ogre->render();
}

void SceneNormalMap::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneNormalMap::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
//    projection = glm::perspective(50.0f, (float)w/h, 0.3f, 100.0f);
    float c = 2.0f;
    projection = glm::ortho( -0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
}

void SceneNormalMap::compileAndLinkShader()
{
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/normalmap.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.Log().c_str());
        exit(1);
    }
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/normalmap.fs",GLSLShader::FRAGMENT))
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
