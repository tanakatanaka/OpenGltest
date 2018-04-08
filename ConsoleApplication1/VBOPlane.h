#pragma once

#include "drawable.h"

class VBOPlane : public Drawable
{
private:
    unsigned int vaoHandle;
    int faces;

public:
    VBOPlane(float, float, int, int);

    void render() const;
};