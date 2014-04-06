
#ifndef POLY_MESH_H
#define POLY_MESH_H


#include "Globals.h"


namespace Poly
{


class Mesh : private boost::noncopyable
{
public:
    Mesh();
    virtual ~Mesh();

protected:
    struct Vertex
    {
        std::array<float,3> Position;
        std::array<float,4> Color;
    };
};


}


#endif //POLY_MESH_H
