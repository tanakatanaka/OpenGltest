#include "stdafx.h"
#include "scenerefractcube.h"
#include <cstdio>
#include <cstdlib>
#include <SDL_image.h>
#include <iostream>
using std::cout;
using std::endl;

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

SceneRefractCube::SceneRefractCube()
{
}

void SceneRefractCube::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(14, mat4(1.0f));
    sky = new SkyBox();
    plane = new VBOPlane(1.0f,1.0f,1,1);
    float c = 3.5f;
    torus = new VBOTorus(0.7f * c, 0.3f * c, 50, 50);

    projection = mat4(1.0f);

    angle = (float)( TO_RADIANS(90.0) );

    loadCubeMap("../texture/debevec/stpeters_cross");
}

void SceneRefractCube::loadCubeMap( const char * baseFileName )
{
    glActiveTexture(GL_TEXTURE0);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    GLuint targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for( int i = 0; i < 6; i++ ) 
	{
        string texName = string(baseFileName) + "_" + suffixes[i] + ".jpg";
        cout << "Loading: " << texName << endl;

		SDL_Surface *hoge = IMG_Load(texName.c_str());
		auto img = convert_to_opengl_format(hoge);

        //QImage img = QGLWidget::convertToGLFormat(QImage(texName.c_str(),"PNG"));
        glTexImage2D(targets[i], 0, GL_RGBA, img->w, img->h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    prog.setUniform("CubeMapTex", 0);

}

void SceneRefractCube::update( float t )
{
    angle += 0.001f;
    if( angle > TWOPI) angle -= TWOPI;

	auto state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_LEFT]) { angle -= 0.05; }
	if (state[SDL_SCANCODE_RIGHT]) { angle += 0.05; }
	if (state[SDL_SCANCODE_UP]) { angle += 0.05; }
	if (state[SDL_SCANCODE_DOWN]) { angle += 0.05; }
}

void SceneRefractCube::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3( 7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,-1.0f,0.0f));
    prog.setUniform("WorldCameraPosition", cameraPos);
    //view = glm::lookAt(vec3(0.0f,2.0f,0.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,0.0f,1.0f));

    prog.setUniform("DrawSkyBox", true);
    model = mat4(1.0f);
    setMatrices();
    sky->render();
    prog.setUniform("DrawSkyBox", false);

    prog.setUniform("Material.Eta", 0.94f);
    prog.setUniform("Material.ReflectionFactor", 0.1f);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,-1.0f,0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();
}

void SceneRefractCube::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelMatrix", model);
   // prog.setUniform("ModelViewMatrix", mv);
    //prog.setUniform("NormalMatrix",
    //                mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneRefractCube::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(200.0f, (float)w/h, 0.3f, 100.0f);
    //float c = 2.0f;
    //projection = glm::ortho( -0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
}

void SceneRefractCube::compileAndLinkShader()
{
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/cubemap_refract.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.Log().c_str());
        exit(1);
    }
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/cubemap_refract.fs",GLSLShader::FRAGMENT))
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
