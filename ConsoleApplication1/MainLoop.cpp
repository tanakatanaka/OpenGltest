#include "stdafx.h"
#include "MainLoop.h"
#include <stdio.h>
#include <glew.h>
#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/glu.h>

void MainLoop::InitGlew()
{
	GLenum err = glewInit();
	
	if( GLEW_OK != err )
	{
		printf((char *)glewGetErrorString(err));
	}
}