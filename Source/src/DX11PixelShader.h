
#ifndef POLY_DX11_PIXEL_SHADER_H
#define POLY_DX11_PIXEL_SHADER_H


#include "Globals.h"
#include "DX11.h"


namespace Poly
{


class DX11PixelShader : private boost::noncopyable
{
public:
    DX11PixelShader();
    virtual ~DX11PixelShader();

    void Initialize(const string& shaderFileName,ID3D11Device* pDevice);

    void Render(ID3D11DeviceContext* pDeviceContext);

private:
    /*The real pixel shader object.*/
    ID3D11PixelShader* mpPixelShader;
};


}


#endif //POLY_DX11_PIXEL_SHADER_H
