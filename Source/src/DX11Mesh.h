
#ifndef POLY_DX11_MESH_H
#define POLY_DX11_MESH_H


#include "Globals.h"
#include "DX11.h"
#include "Mesh.h"


namespace Poly
{


class DX11Mesh : public Mesh
{
public:
    DX11Mesh();
    virtual ~DX11Mesh();

    void Initialize(ID3D11Device* pDevice);

    void Render(ID3D11DeviceContext* pDeviceContext);

private:
    uint mNumIndices;
    uint mNumVertices;

    /*The buffer that stores index data.*/
    ID3D11Buffer* mpIndexBuffer;

    /*The buffer that stores vertex data.*/
    ID3D11Buffer* mpVertexBuffer;
};


}


#endif //POLY_DX11_MESH_H
