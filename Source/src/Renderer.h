
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

    virtual void CalculateOrthographicMatrix(float width,float height,
        float nearPlane,float farPlane);
    virtual void CalculatePerspectiveMatrix(float width,float height,
        float nearPlane,float farPlane,float fovY);

protected:
    float mFps;
    int mFpsIterations;
    float mFpsLastTimestamp;
    bool mFpsLogging;

    string mShadersPath;

    bool mVerticalSync;

    /*Matrix that defines the clipping planes (forming a box) used for the
      orthographic projection (2D UI rendering).*/
    Matrix4DA mOrthographicMatrix;

    /*Matrix that defines the field of view and the clipping planes (forming a
      frustum) used for the perspective projection (3D rendering).*/
    Matrix4DA mPerspectiveMatrix;

    /*Matrix that transforms vertices of objects from their local space to world
      space; used for translations, rotations and scaling.*/
    Matrix4DA mWorldMatrix;
};


}


#endif //POLY_RENDERER_H
