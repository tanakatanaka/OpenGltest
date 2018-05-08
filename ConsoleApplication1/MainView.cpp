#include "stdafx.h"
#include "mainview.h"
#include "scenequadtess.h"
#include "scenetessteapot.h"
#include "sceneblur.h"
#include "sceneedge.h"
#include "scenebloom.h"
#include "scenerendertotex.h"
#include "scenegamma.h"
#include "scenemsaa.h"
#include "scenepointsprite.h"
#include "sceneshadewire.h"

#include "scenebezcurve.h"
#include "scenemultitex.h"
#include "scenealpha.h"
#include "scenerefractcube.h"
#include"scenereflectcube.h"
#include "sceneprojtex.h"
#include "scenespot.h"
#include "sceneAds.h"
#include "sceneToon.h"
#include"sceneFog.h"
#include"sceneTexture.h"
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

#include "scenesilhouette.h"
#include "scenedeferred.h"
#include "scenenormalmap.h"
#include "scenefragment.h"
#include "glutils.h"

#include <iostream>
using std::cout;
using std::endl;
#include <cstdio>

void  MainView::InitializeGL()
{
	scene.reset(new SceneSilhouette());

	glClearColor(0.2f,0.2f,0.2f,1.0f);
	scene->initScene();
}

void MainView::paintGL() 
{
	scene->update(0.0f);
    GLUtils::checkForOpenGLError(__FILE__,__LINE__);
	scene->render();
}

void MainView::resizeGL(int w, int h )
{
    scene->resize(w,h);
}


