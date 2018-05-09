#include "stdafx.h"
#include "sceneshadowmap.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneShadowMap::SceneShadowMap()
{
	width = 800;
	height = 600;
	shadowMapWidth = 512;
	shadowMapHeight = 512;
}

void SceneShadowMap::initScene()
{
	compileAndLinkShader();

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	angle = PI / 4.0f;

	teapot.reset(new VBOTeapot(14, mat4(1.0f)));
	plane.reset(new VBOPlane(40.0f, 40.0f, 2, 2));
	float scale = 2.0f;
	torus.reset(new VBOTorus(0.7f * scale, 0.3f * scale, 50, 50));
	// Set up the framebuffer object
	setupFBO();

	GLuint programHandle = prog.GetHandle();
	pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "recordDepth");
	pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "shadeWithShadow");

	shadowBias = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, 0.5f, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, 0.5f, 0.0f),
		vec4(0.5f, 0.5f, 0.5f, 1.0f)
	);

	lightFrustum.reset(new Frustum(Projection::PERSPECTIVE));
	float c = 1.65f;
	vec3 lightPos = vec3(0.0f, c * 5.25f, c * 7.5f);  // World coords
	lightFrustum->orient(lightPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
	lightFrustum->setPerspective(50.0f, 1.0f, 1.0f, 25.0f);
	lightPV = shadowBias * lightFrustum->getProjectionMatrix() * lightFrustum->getViewMatrix();

	prog.setUniform("Light.Intensity", vec3(0.85f));

	prog.setUniform("ShadowMap", 0);

	glLineWidth(4.5f);
	glPointSize(3.5f);
}

void SceneShadowMap::spitOutDepthBuffer() {

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

	int size = shadowMapWidth * shadowMapHeight;
	auto buffer = std::make_unique<float[]>(size);
	auto imgBuffer = std::make_unique<Uint8[]>(size * 4);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, buffer.get());

	for (int i = 0; i < shadowMapHeight; i++)
		for (int j = 0; j < shadowMapWidth; j++)
		{
			int imgIdx = 4 * ((i*shadowMapWidth) + j);
			int bufIdx = ((shadowMapHeight - i - 1) * shadowMapWidth) + j;

			// This is just to make a more visible image.  Scale so that
			// the range (minVal, 1.0) maps to (0.0, 1.0).  This probably should
			// be tweaked for different light configurations.
			float minVal = 0.88f;
			float scale = (buffer[bufIdx] - minVal) / (1.0f - minVal);
			unsigned char val = (unsigned char)(scale * 255);
			imgBuffer[imgIdx] = val;
			imgBuffer[imgIdx + 1] = val;
			imgBuffer[imgIdx + 2] = val;
			imgBuffer[imgIdx + 3] = 0xff;
		}

	auto img = SDL_SurfacePtr(SDL_CreateRGBSurfaceWithFormatFrom(imgBuffer.get(), shadowMapWidth, shadowMapHeight, 32, 4 * shadowMapWidth, SDL_PIXELFORMAT_RGBA32));
	IMG_SavePNG(img.get(), "../depth.png");

	exit(1);
}

void SceneShadowMap::setupFBO()
{
	GLfloat border[] = { 1.0f, 0.0f,0.0f,0.0f };
	// The depth buffer texture
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth,
		shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	// Assign the depth buffer texture to texture channel 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTex);

	// Create and set up the FBO
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTex, 0);

	GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is complete.\n");
	}
	else {
		printf("Framebuffer is not complete.\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneShadowMap::update(float t)
{
	angle += 0.003f;
	if (angle > TWOPI) angle -= TWOPI;

}

void SceneShadowMap::render()
{
	static int i = 0;
	prog.Use();
	// Pass 1 (shadow map generation)
	view = lightFrustum->getViewMatrix();
	projection = lightFrustum->getProjectionMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, shadowMapWidth, shadowMapHeight);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	drawScene();
	glFlush();
	glFinish();
	auto state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_RETURN])
	{
		spitOutDepthBuffer();
	}

	// Pass 2 (render)
	float c = 1.0f;
	vec3 cameraPos(c * 51.5f * cos(angle), c * 7.0f, c * 51.5f * sin(angle));
	view = glm::lookAt(cameraPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
	prog.setUniform("Light.Position", view * vec4(lightFrustum->getOrigin(), 1.0f));
	projection = glm::perspective(100.0f, (float)width / height, 0.1f, 100.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
	glDisable(GL_CULL_FACE);
	drawScene();

	// Uncomment to draw the light's frustum
	if (state[SDL_SCANCODE_SPACE])
	{
		solidProg.Use();
		solidProg.setUniform("Color", vec4(1.0f, 0.0f, 0.0f, 1.0f));
		model = mat4(1.0f);
		mat4 mv = view * model;
		solidProg.setUniform("MVP", projection * mv);
		lightFrustum->render();
	}
}

void SceneShadowMap::drawScene()
{
	vec3 color = vec3(0.7f, 0.5f, 0.3f);
	prog.setUniform("Material.Ka", color * 0.05f);
	prog.setUniform("Material.Kd", color);
	prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
	prog.setUniform("Material.Shininess", 150.0f);
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0f, 0.0f, 0.0f));
	model *= glm::rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
	setMatrices();
	teapot->render();

	prog.setUniform("Material.Ka", color * 0.05f);
	prog.setUniform("Material.Kd", color);
	prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
	prog.setUniform("Material.Shininess", 150.0f);
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0f, 2.0f, 5.0f));
	model *= glm::rotate(-45.0f, vec3(1.0f, 0.0f, 0.0f));
	setMatrices();
	torus->render();

	prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
	prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
	prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
	prog.setUniform("Material.Shininess", 1.0f);
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0f, 0.0f, 0.0f));
	setMatrices();
	plane->render();
	model = mat4(1.0f);
	model *= glm::translate(vec3(-5.0f, 5.0f, 0.0f));
	model *= glm::rotate(-90.0f, vec3(0.0f, 0.0f, 1.0f));
	setMatrices();
	plane->render();
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0f, 5.0f, -5.0f));
	model *= glm::rotate(90.0f, vec3(1.0f, 0.0f, 0.0f));
	setMatrices();
	plane->render();
	model = mat4(1.0f);
}

void SceneShadowMap::setMatrices()
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix",
		mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	prog.setUniform("MVP", projection * mv);
	prog.setUniform("ShadowMatrix", lightPV * model);
}

void SceneShadowMap::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

void SceneShadowMap::compileAndLinkShader()
{
	if (!prog.CompileShaderFromFile("../ConsoleApplication1/Shader/shadowmap.vs", GLSLShader::VERTEX))
	{
		printf("Vertex shader failed to compile!\n%s",
			prog.Log().c_str());
		exit(1);
	}
	if (!prog.CompileShaderFromFile("../ConsoleApplication1/Shader/shadowmap.fs", GLSLShader::FRAGMENT))
	{
		printf("Fragment shader failed to compile!\n%s",
			prog.Log().c_str());
		exit(1);
	}
	if (!prog.Link())
	{
		printf("Shader program failed to link!\n%s",
			prog.Log().c_str());
		exit(1);
	}

	prog.Use();

	if (!solidProg.CompileShaderFromFile("../ConsoleApplication1/Shader/solid.vs", GLSLShader::VERTEX))
	{
		printf("Vertex shader failed to compile!\n%s",
			solidProg.Log().c_str());
		exit(1);
	}
	if (!solidProg.CompileShaderFromFile("../ConsoleApplication1/Shader/solid.fs", GLSLShader::FRAGMENT))
	{
		printf("Fragment shader failed to compile!\n%s",
			solidProg.Log().c_str());
		exit(1);
	}
	if (!solidProg.Link())
	{
		printf("Shader program failed to link!\n%s",
			solidProg.Log().c_str());
		exit(1);
	}
}
