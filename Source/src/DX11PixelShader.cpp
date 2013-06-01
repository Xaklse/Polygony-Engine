
#include "DX11PixelShader.h"


#include "Exception.h"


namespace Poly
{


DX11PixelShader::DX11PixelShader() :
    mpPixelShader(nullptr)
{
}

DX11PixelShader::~DX11PixelShader()
{
    if (mpPixelShader != nullptr)
    {
        mpPixelShader->Release();
        mpPixelShader = nullptr;
    }
}

void DX11PixelShader::Initialize(const string& shaderFileName,
    ID3D11Device* pDevice)
{
    //Interface used to return arbitrary length data.
    ID3D10Blob* pPixelShaderBlob;

    //Load and compile the pixel shader.
    HRESULT result = D3DX11CompileFromFile(
        (std::wstring(shaderFileName.begin(),shaderFileName.end())).c_str(),
        nullptr,nullptr,"Main","ps_5_0",0,0,nullptr,&pPixelShaderBlob,nullptr,
        nullptr);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CompilePixelShader)." +
            DEBUG_INFO,result);
    }

    //Encapsulate the pixel shader into a shader object.
    result = pDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(),
        pPixelShaderBlob->GetBufferSize(),nullptr,&mpPixelShader);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreatePixelShader)." +
            DEBUG_INFO,result);
    }
}

void DX11PixelShader::Render(ID3D11DeviceContext* pDeviceContext)
{
    //Set the pixel shader object as active and ready for the device.
    pDeviceContext->PSSetShader(mpPixelShader,nullptr,0);
}


}
