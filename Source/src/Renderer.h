
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

    virtual bool FrameResize() = 0;
    virtual void Render() = 0;

protected:
    float mFps;
    int mFpsIterations;
    float mFpsLastTimestamp;
    bool mFpsLogging;

    bool mVerticalSync;
};


}


#endif //POLY_RENDERER_H
