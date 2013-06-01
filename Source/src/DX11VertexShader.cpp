
#include "DX11VertexShader.h"


#include "Exception.h"


namespace Poly
{


DX11VertexShader::DX11VertexShader() :
    mpInputLayout(nullptr),
    mpVertexShader(nullptr)
{
}

DX11VertexShader::~DX11VertexShader()
{
    if (mpInputLayout != nullptr)
    {
        mpInputLayout->Release();
        mpInputLayout = nullptr;
    }

    if (mpVertexShader != nullptr)
    {
        mpVertexShader->Release();
        mpVertexShader = nullptr;
    }
}

void DX11VertexShader::Initialize(const string& shaderFileName,
    ID3D11Device* pDevice)
{
    //Interface used to return arbitrary length data.
    ID3D10Blob* pVertexShaderBlob;

    //Load and compile the vertex shader.
    HRESULT result = D3DX11CompileFromFile(
        (std::wstring(shaderFileName.begin(),shaderFileName.end())).c_str(),
        nullptr,nullptr,"Main","vs_5_0",0,0,nullptr,&pVertexShaderBlob,nullptr,
        nullptr);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CompileVertexShader)." +
            DEBUG_INFO,result);
    }

    //Encapsulate the vertex shader into a shader object.
    result = pDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(),
        pVertexShaderBlob->GetBufferSize(),nullptr,&mpVertexShader);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateVertexShader)." +
            DEBUG_INFO,result);
    }

    //Initialize the descriptor of the input-layout object.
    D3D11_INPUT_ELEMENT_DESC inputElementDescriptor[] =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
         D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
         D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
    };

    //Create the input-layout object.
    result = pDevice->CreateInputLayout(inputElementDescriptor,2,
        pVertexShaderBlob->GetBufferPointer(),
        pVertexShaderBlob->GetBufferSize(),&mpInputLayout);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateInputLayout)." +
            DEBUG_INFO,result);
    }
}

void DX11VertexShader::Render(ID3D11DeviceContext* pDeviceContext)
{
    //Set the vertex shader object as active and ready for the device.
    pDeviceContext->VSSetShader(mpVertexShader,nullptr,0);

    //Bind the input-layout object to the input-assembler stage.
    pDeviceContext->IASetInputLayout(mpInputLayout);
}


}
