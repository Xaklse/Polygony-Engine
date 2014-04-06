
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

    void Render(ID3D11DeviceContext* pDeviceContext,const Matrix4& worldMatrix,
        const Matrix4& viewMatrix,const Matrix4& projectionMatrix);

private:
    /*Mirrors a buffer declared in the vertex shader .hlsl file.*/
    struct MatricesBuffer
    {
        std::array<float,16> Projection;
        std::array<float,16> View;
        std::array<float,16> World;
    };

    /*The input-layout interface holds a definition of how to feed vertex data
      that is laid out in memory into the input-assembler stage.*/
    ID3D11InputLayout* mpInputLayout;

    /*The buffer that stores the important matrices.*/
    ID3D11Buffer* mpMatricesBuffer;

    /*The real vertex shader object.*/
    ID3D11VertexShader* mpVertexShader;
};


}


#endif //POLY_DX11_VERTEX_SHADER_H
