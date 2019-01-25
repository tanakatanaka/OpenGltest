#pragma once
#include "stdafx.h"
#include <glew.h>
#include <memory>
#include "scene.h"

class MainView
{
private:
    std::unique_ptr<Scene> scene;
	bool IsEnd = false;

	enum SceneId
	{
		MINID = 1,
		TOON = MINID,
		EDGE,
		SPOT,
		BLOOM,
		MAXCOUNT
	};


public:
    void InitializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void toggleAnimation();
    void takeScreenShot();
};

