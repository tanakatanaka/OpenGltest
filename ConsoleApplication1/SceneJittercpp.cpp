#include "stdafx.h"
#include "scenejitter.h"
#include <cstdio>

#include "glutils.h"
#include "defines.h"
using glm::vec3;
using glm::vec2;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneJitter::SceneJitter()
{
    width = 800;
    height = 600;
    shadowMapWidth = 512;
    shadowMapHeight = 512;

    samplesU = 4;
    samplesV = 8;
    jitterMapSize = 8;
    radius = 7.0f;
}

void SceneJitter::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    //angle = PI/4.0f;
    angle = TWOPI * 0.85;

    teapot = new VBOTeapot(14, mat4(1.0f));
    plane = new VBOPlane(20.0f, 20.0f, 2, 2);
    float scale = 2.0f;
    torus = new VBOTorus(0.7f * scale,0.3f * scale,50,50);
    mesh = new VBOMesh("../ConsoleApplication1/mesh/building.obj");

    // Set up the framebuffer object
    setupFBO();
    buildJitterTex();

    GLuint programHandle = prog.GetHandle();
    pass1Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "recordDepth");
    pass2Index = glGetSubroutineIndex( programHandle, GL_FRAGMENT_SHADER, "shadeWithShadow");

    shadowScale = mat4( vec4(0.5f,0.0f,0.0f,0.0f),
                        vec4(0.0f,0.5f,0.0f,0.0f),
                        vec4(0.0f,0.0f,0.5f,0.0f),
                        vec4(0.5f,0.5f,0.5f,1.0f)
                        );

//    lightPos = vec3(0.0f,10.5f,15.0f);  // World coords
//    lightView = glm::lookAt(lightPos, vec3(0.0f), vec3(0.0f,1.0f,0.0f));
//    lightProjection = glm::perspective(50.0f, 1.0f, 0.1f, 100.0f);
//    lightPV = shadowScale * lightProjection * lightView;

    lightFrustum = new Frustum(Projection::PERSPECTIVE);
    lightPos = vec3(-2.5f,2.0f,-2.5f);  // World coords
    lightFrustum->orient(lightPos,vec3(0.0f),vec3(0.0f,1.0f,0.0f));
    lightFrustum->setPerspective(40.0f, 1.0f, 0.1f, 100.0f);

    lightPV = shadowScale * lightFrustum->getProjectionMatrix() * lightFrustum->getViewMatrix();

    prog.setUniform("Light.Intensity", vec3(0.85f));

    prog.setUniform("ShadowMap", 0);
    prog.setUniform("OffsetTex", 1);
    prog.setUniform("Radius", radius / 512.0f);
    prog.setUniform("OffsetTexSize", vec3(jitterMapSize,jitterMapSize, samplesU * samplesV / 2.0f));
}

void SceneJitter::setupFBO()
{
    GLfloat border[] = {1.0f, 0.0f,0.0f,0.0f };
    // The depth buffer texture
    GLuint depthTex;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth,
                 shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    // Assign the depth buffer texture to texture channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex);

    // Create and set up the FBO
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, depthTex, 0);

    GLenum drawBuffers[] = {GL_NONE};
    glDrawBuffers(1, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    } else {
        printf("Framebuffer is not complete.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void SceneJitter::buildJitterTex()
{
    int size = jitterMapSize;
    int samples = samplesU * samplesV;
    int bufSize = size * size * samples * 2;
    float *data = new float[bufSize];

    for( int i = 0; i < size; i++ ) {
        for(int j = 0; j < size; j++ ) {
            for( int k = 0; k < samples; k += 2 ) {
                int x1,y1,x2,y2;
                x1 = k % (samplesU);
                y1 = (samples - 1 - k) / samplesU;
                x2 = (k+1) % samplesU;
                y2 = (samples - 1 - k - 1) / samplesU;

                vec4 v;
                // Center on grid and jitter
                v.x = (x1 + 0.5f) + jitter();
                v.y = (y1 + 0.5f) + jitter();
                v.z = (x2 + 0.5f) + jitter();
                v.w = (y2 + 0.5f) + jitter();

                // Scale between 0 and 1
                v.x /= samplesU;
                v.y /= samplesV;
                v.z /= samplesU;
                v.w /= samplesV;

                // Warp to disk
                int cell = ((k/2) * size * size + j * size + i) * 4;
                data[cell+0] = sqrtf(v.y) * cosf( TWOPI * v.x );
                data[cell+1] = sqrtf(v.y) * sinf( TWOPI * v.x );
                data[cell+2] = sqrtf(v.w) * cosf( TWOPI * v.z );
                data[cell+3] = sqrtf(v.w) * sinf( TWOPI * v.z );
            }
        }
    }

    glActiveTexture(GL_TEXTURE1);
    GLuint texID;
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_3D, texID);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, size, size, samples/2, 0, GL_RGBA, GL_FLOAT, data);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    delete [] data;
}

// Return random float between -0.5 and 0.5
float SceneJitter::jitter() {
    return ((float)rand() / RAND_MAX) - 0.5f;
}

void SceneJitter::update( float t )
{
    angle += 0.003f;
    if( angle > TWOPI) angle -= TWOPI;
}

void SceneJitter::render()
{
    // Pass 1 (shadow map generation)
    view = lightFrustum->getViewMatrix();
    projection = lightFrustum->getProjectionMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,shadowMapWidth,shadowMapHeight);
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass1Index);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.5f,10.0f);
    drawBuildingScene();
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Pass 2 (render)
//    vec3 cameraPos(11.5f * cos(angle),7.0f,11.5f * sin(angle));
//    view = glm::lookAt(cameraPos,vec3(0.0f),vec3(0.0f,1.0f,0.0f));
//    prog.setUniform("Light.Position", view * vec4(lightPos,1.0f));
//    projection = glm::perspective(50.0f, (float)width/height, 0.1f, 100.0f);

    //vec3 cameraPos(1.8f * cos(angle),0.7f,1.8f * sin(angle));
    //view = glm::lookAt(cameraPos,vec3(0.0f,-0.175f,0.0f),vec3(0.0f,1.0f,0.0f));

	vec3 cameraPos = vec3( 7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,-1.0f,0.0f));

    prog.setUniform("Light.Position", view * vec4(lightFrustum->getOrigin(),1.0));
	projection = glm::perspective(200.0f, (float)width/height, 0.3f, 1000.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,width,height);
    glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &pass2Index);
    glDisable(GL_CULL_FACE);
    drawBuildingScene();
    glFinish();
}

void SceneJitter::drawBuildingScene()
{
    vec3 color = vec3(1.0f,0.85f,0.55f);
    prog.setUniform("Material.Ka", color * 0.1f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.0f));
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,0.0f,0.0f));
    //model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    mesh->render();

    prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,0.0f,0.0f));
    setMatrices();
    plane->render();
}

void SceneJitter::drawScene()
{
    vec3 color = vec3(0.7f,0.5f,0.3f);
    prog.setUniform("Material.Ka", color * 0.05f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.9f,0.9f,0.9f));
    prog.setUniform("Material.Shininess", 150.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,0.0f,0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Material.Ka", color * 0.05f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.9f,0.9f,0.9f));
    prog.setUniform("Material.Shininess", 150.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,2.0f,5.0f));
    model *= glm::rotate(-45.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    torus->render();

    prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,0.0f,0.0f));
    setMatrices();
    plane->render();
    model = mat4(1.0f);
    model *= glm::translate(vec3(-5.0f,5.0f,0.0f));
    model *= glm::rotate(-90.0f,vec3(0.0f,0.0f,1.0f));
    setMatrices();
    plane->render();
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,5.0f,-5.0f));
    model *= glm::rotate(90.0f,vec3(1.0f,0.0f,0.0f));
    setMatrices();
    plane->render();
    model = mat4(1.0f);
}

void SceneJitter::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ShadowMatrix", lightPV * model);
}

void SceneJitter::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneJitter::compileAndLinkShader()
{
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/jitter.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.Log().c_str());
        exit(1);
    }
    if( ! prog.CompileShaderFromFile("../ConsoleApplication1/Shader/jitter.fs",GLSLShader::FRAGMENT))
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
