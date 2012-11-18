
#ifndef POLY_RENDERER_H
#define POLY_RENDERER_H


#include "Globals.h"


namespace Poly
{


class System;


class Renderer : private boost::noncopyable
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void Initialize(uint width,uint height,bool fullScreen,
        bool verticalSync);

    virtual void Render();
    virtual bool WindowResize();
};


}


#include "System.h"


#endif //POLY_RENDERER_H
