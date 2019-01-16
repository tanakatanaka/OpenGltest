#pragma once
#include "stdafx.h"
#include <glew.h>
#include <memory>
#include "scene.h"

class MainView
{
private:
    std::unique_ptr<Scene> scene;

	enum SceneId
	{
		MINID = 1,
		TOON = MINID,
		SHADOW,
		SHADOWMAP,
		MAXCOUNT
	};


public:
    void InitializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void toggleAnimation();
    void takeScreenShot();
};

