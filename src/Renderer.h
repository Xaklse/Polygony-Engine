
#ifndef POLY_RENDERER_H
#define POLY_RENDERER_H


#include "Globals.h"


class System;


class Renderer
{
public:
    Renderer(System* pSystem);
    virtual ~Renderer();

    virtual bool Initialize();

    virtual bool Render();

protected:
    System* mpSystem;
};


#include "System.h"


#endif //POLY_RENDERER_H
