
#ifndef POLY_RENDERER_H
#define POLY_RENDERER_H


#include "Globals.h"


namespace Poly
{


class Renderer : private boost::noncopyable
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void Initialize(uint width,uint height,bool fullScreen) = 0;

    virtual void Render() = 0;
    virtual bool WindowResize() = 0;
};


}


#endif //POLY_RENDERER_H
