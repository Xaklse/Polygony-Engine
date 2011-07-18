
#ifndef PE_RENDERER_H
#define PE_RENDERER_H


#include "Globals.h"


class System;


class Renderer
{
public:
    Renderer(System* system);
    virtual ~Renderer();

    virtual bool Initialize();

    virtual bool Render();

protected:
    System* mSystem;
};


#include "System.h"


#endif //PE_RENDERER_H
