
#ifndef POLY_SCENE_CAMERA_H
#define POLY_SCENE_CAMERA_H


#include "Globals.h"


namespace Poly
{


class SceneCamera
{
public:
    SceneCamera();
    virtual ~SceneCamera();

    Matrix4 Render() const;
};


}


#endif //POLY_SCENE_CAMERA_H
