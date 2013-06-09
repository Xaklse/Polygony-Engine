
#include "DX11PixelShader.h"


#include <D3DCompiler.h>


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
    //Force strict compile, which might not allow for legacy syntax.
    UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef POLY_DEBUG_SHADERS
    //Insert debug file/line/type/symbol information into the output code.
    compileFlags |= D3DCOMPILE_DEBUG;

    //Treat all warnings as errors.
    compileFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif

    //Interfaces used to return arbitrary length data.
    ID3D10Blob* pErrorMessage;
    ID3D10Blob* pPixelShaderBlob;

    //Load and compile the pixel shader.
    HRESULT result = D3DX11CompileFromFile(
        (std::wstring(shaderFileName.begin(),shaderFileName.end())).c_str(),
        nullptr,nullptr,"Main","ps_5_0",compileFlags,0,nullptr,
        &pPixelShaderBlob,&pErrorMessage,nullptr);

    if (FAILED(result))
    {
        if (pErrorMessage != nullptr)
        {
            //Log the shader compilation errors.
            string compileErrors((char*)(pErrorMessage->GetBufferPointer()));

            pErrorMessage->Release();

            throw Exception("Direct3D 11 init failed (CompilePixelShader): " +
                compileErrors + "." + DEBUG_INFO,result);
        }
        else
        {
            throw Exception("Direct3D 11 init failed (CompilePixelShader)." +
                DEBUG_INFO,result);
        }
    }

    //Encapsulate the pixel shader into a shader object.
    result = pDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(),
        pPixelShaderBlob->GetBufferSize(),nullptr,&mpPixelShader);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreatePixelShader)." +
            DEBUG_INFO,result);
    }

    //This object is no longer needed.
    pPixelShaderBlob->Release();
}

void DX11PixelShader::Render(ID3D11DeviceContext* pDeviceContext)
{
    //Set the pixel shader object as active and ready for the device.
    pDeviceContext->PSSetShader(mpPixelShader,nullptr,0);
}


}
