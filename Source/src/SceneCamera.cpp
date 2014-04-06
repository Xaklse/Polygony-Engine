
#include "SceneCamera.h"


namespace Poly
{


SceneCamera::SceneCamera()
{
}

SceneCamera::~SceneCamera()
{
}

Matrix4 SceneCamera::Render() const
{
    Matrix4 matrix;

    //Create a look-at matrix for camera; uses a left-handed coordinate system.
    //    /     u.x        v.x        w.x     0  \
    //    |     u.y        v.y        w.y     0  |
    //    |     u.z        v.z        w.z     0  |
    //    \  -dot(u,p)  -dot(v,p)  -dot(w,p)  1  /
    //The right (u), up (v) and forward (w) vectors describe the orientation
    //of the coordinate axes for camera space in terms of world space.
    //p is the position (vector) of the camera; dot is the dot product.
    matrix << 0.0f,0.0f,1.0f,0.0f,
              1.0f,0.0f,0.0f,0.0f,
              0.0f,1.0f,0.0f,0.0f,
              0.0f,0.0f,0.0f,1.0f;

    return matrix;
}


}
