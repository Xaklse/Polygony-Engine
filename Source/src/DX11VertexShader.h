
#ifndef POLY_DX11_VERTEX_SHADER_H
#define POLY_DX11_VERTEX_SHADER_H


#include "Globals.h"
#include "DX11.h"


namespace Poly
{


class DX11VertexShader : private boost::noncopyable
{
public:
    DX11VertexShader();
    virtual ~DX11VertexShader();

    void Initialize(const string& shaderFileName,ID3D11Device* pDevice);

    void Render(ID3D11DeviceContext* pDeviceContext);

private:
    struct MatricesBuffer
    {
        D3DXMATRIX World;
        D3DXMATRIX View;
        D3DXMATRIX Projection;
    };

    ID3D11InputLayout* mpInputLayout;
    ID3D11Buffer* mpMatricesBuffer;
    ID3D11VertexShader* mpVertexShader;
};


}


#endif //POLY_DX11_VERTEX_SHADER_H
