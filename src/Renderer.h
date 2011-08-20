
#ifndef POLY_RENDERER_H
#define POLY_RENDERER_H


#include "Globals.h"


namespace Poly
{


class System;


class Renderer
{
public:
    Renderer(System* pSystem);
    virtual ~Renderer();

    virtual bool Initialize(bool fullScreen,unsigned int width,
        unsigned int height);

    virtual bool Render();
    virtual bool WindowResize();

protected:
    System* mpSystem;
};


}


#include "System.h"


#endif //POLY_RENDERER_H
