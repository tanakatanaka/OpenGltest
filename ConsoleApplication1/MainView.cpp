#include "stdafx.h"
#include "mainview.h"

#include "sceneAds.h"
/*
#include "scenebasic.h"
#include "sceneDiscard.h"
#include "sceneDiffuse.h"
#include "sceneAds.h"
#include "sceneFlat.h"
#include "sceneTwoside.h"
#include "sceneMulti.h"
#include "sceneUni.h"
#include "sceneSub.h"
#include "scenedirectional.h"
*/
#include "scenefragment.h"
#include "glutils.h"

#include <iostream>
using std::cout;
using std::endl;
#include <cstdio>

void  MainView::InitializeGL()
{
	scene = new SceneADS();

	glClearColor(0.2f,0.2f,0.2f,1.0f);
	scene->initScene();
}

void MainView::paintGL() 
{
	//scene->update(0.0f);
    GLUtils::checkForOpenGLError(__FILE__,__LINE__);
	scene->render();
}

void MainView::resizeGL(int w, int h )
{
    scene->resize(w,h);
}


