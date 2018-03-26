// ConsoleApplication1.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "sdl.h"
#include "MainLoop.h"
#include "Shader.h"

int main(int argc, char *args[])
{
	SDL_Window *window;
    //SDL_Renderer *renderer;
    //SDL_Surface *surface;
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


	window = SDL_CreateWindow("SDL2/OpenGL Demo", 0, 0, 640, 480, SDL_WINDOW_OPENGL);

	if (window == NULL) 
	{
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ウィンドウとレンダラーを生成できなかった: %s", SDL_GetError());
    return 3;
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	MainLoop mainLoop;
	Shader  shader;
	
	const char*msg = SDL_GetError();
	printf(msg);

	mainLoop.InitGlew();
	shader.Load("../Shader/basic.frag");

	while (1)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) 
		{
			break;
		}
		/*
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		//SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		*/
	}

    //SDL_DestroyTexture(texture);
    //SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

