#pragma once
#include "drawable.h"

class SkyBox : public Drawable
{
private:
    unsigned int vaoHandle;

public:
    SkyBox();

    void render() const;
};
