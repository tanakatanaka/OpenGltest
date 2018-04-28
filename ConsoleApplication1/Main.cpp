// ConsoleApplication1.cpp : コンソール アプリケーションのエントリ ポイントを定義します。

#include "stdafx.h"
#include "sdl.h"
#include "MainLoop.h"
#include "Shader.h"
#include "SDL_opengl.h"

#include "mainview.h"
#include <memory>
#include <functional>

struct Quit
{
	~Quit()
	{
		SDL_Quit();
	}
};

int main(int argc, char *args[])
{
	Quit quit;
	auto window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>(nullptr, SDL_DestroyWindow);
	SDL_Event event;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	
	SDL_Init(SDL_INIT_VIDEO);
	window.reset(SDL_CreateWindow("SDL2/OpenGL Demo", 200, 200, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

	if (window == NULL) 
	{
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ウィンドウとレンダラーを生成できなかった: %s", SDL_GetError());
	return 3;
	}

	auto glcontext = std::unique_ptr<void, std::function<void(SDL_GLContext)>>(nullptr, SDL_GL_DeleteContext);
	glcontext.reset(SDL_GL_CreateContext(window.get()));
	MainLoop mainLoop;
	MainView mainView;

		const char *msg = SDL_GetError();
	printf(msg);

	mainLoop.InitGlew();
	mainView.InitializeGL();
	mainView.resizeGL(640, 480);
	glClearColor(1, 0, 0, 1);

	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return 0;
			}
			if (event.type == SDL_WINDOWEVENT &&
				(event.window.event == SDL_WINDOWEVENT_RESIZED))
			{
				mainView.resizeGL(event.window.data1, event.window.data2);
			}

		}
		SDL_GL_SwapWindow(window.get());
		mainView.paintGL();
		SDL_Delay(16);
	}

	return 0;
}

