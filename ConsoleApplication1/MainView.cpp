#include "stdafx.h"
#include "mainview.h"
#include "scenebasic.h"
#include "sceneBox.h"
#include "glutils.h"

#include <iostream>
using std::cout;
using std::endl;
#include <cstdio>

void  MainView::InitializeGL()
{
	scene = new SceneBox();
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	scene->initScene();
}

void MainView::paintGL() 
{
    GLUtils::checkForOpenGLError(__FILE__,__LINE__);
    scene->render();
}

void MainView::resizeGL(int w, int h )
{
    scene->resize(w,h);
}


