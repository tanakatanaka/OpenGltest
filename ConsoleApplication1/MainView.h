#pragma once
#include "stdafx.h"
#include <glew.h>
#include "scene.h"

class MainView
{
private:

    Scene *scene;

public:
    void InitializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void toggleAnimation();
    void takeScreenShot();
};

