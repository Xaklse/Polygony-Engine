
#include "DX11VertexShader.h"


#include <D3DCompiler.h>


#include "Exception.h"


namespace Poly
{


DX11VertexShader::DX11VertexShader() :
    mpInputLayout(nullptr),
    mpMatricesBuffer(nullptr),
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

    if (mpMatricesBuffer != nullptr)
    {
        mpMatricesBuffer->Release();
        mpMatricesBuffer = nullptr;
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
    ID3D10Blob* pVertexShaderBlob;

    //Load and compile the vertex shader.
    HRESULT result = D3DX11CompileFromFile(
        (std::wstring(shaderFileName.begin(),shaderFileName.end())).c_str(),
        nullptr,nullptr,"Main","vs_5_0",compileFlags,0,nullptr,
        &pVertexShaderBlob,&pErrorMessage,nullptr);

    if (FAILED(result))
    {
        if (pErrorMessage != nullptr)
        {
            //Log the shader compilation errors.
            string compileErrors((char*)(pErrorMessage->GetBufferPointer()));

            pErrorMessage->Release();

            throw Exception("Direct3D 11 init failed (CompileVertexShader): " +
                compileErrors + "." + DEBUG_INFO,result);
        }
        else
        {
            throw Exception("Direct3D 11 init failed (CompileVertexShader)." +
                DEBUG_INFO,result);
        }
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

    //This object is no longer needed.
    pVertexShaderBlob->Release();

////////////////////////////////////////////////////////////////////////////////

    D3D11_BUFFER_DESC matricesBufferDescriptor;

    //Initialize the description of the matrices constant buffer.
    ZeroMemory(&matricesBufferDescriptor,sizeof(D3D11_BUFFER_DESC));

    //Fill in the structure with the needed information.
    matricesBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matricesBufferDescriptor.ByteWidth = sizeof(MatricesBuffer);
    matricesBufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matricesBufferDescriptor.MiscFlags = 0;
    matricesBufferDescriptor.StructureByteStride = 0;
    matricesBufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;

    //Create the shader-constant buffer.
    result = pDevice->CreateBuffer(&matricesBufferDescriptor,nullptr,
        &mpMatricesBuffer);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateMatricesBuffer)." +
            DEBUG_INFO,result);
    }
}

void DX11VertexShader::Render(ID3D11DeviceContext* pDeviceContext,
    const Matrix4& worldMatrix,const Matrix4& viewMatrix,
    const Matrix4& projectionMatrix)
{
    //Set the vertex shader object as active and ready for the device.
    pDeviceContext->VSSetShader(mpVertexShader,nullptr,0);

    //Bind the input-layout object to the input-assembler stage.
    pDeviceContext->IASetInputLayout(mpInputLayout);

////////////////////////////////////////////////////////////////////////////////

    D3D11_MAPPED_SUBRESOURCE mappedSubresource;

    //Lock the shader-constant buffer.
    pDeviceContext->Map(mpMatricesBuffer,0,D3D11_MAP_WRITE_DISCARD,0,
        &mappedSubresource);

    //Get a pointer to the data contained in the shader-constant buffer.
    MatricesBuffer* pMatricesData = (MatricesBuffer*)mappedSubresource.pData;

    //Copy the matrices into the shader-constant buffer.
    for (int i = 0; i < projectionMatrix.size(); i++)
    {
        pMatricesData->Projection[i] = *(projectionMatrix.data() + i);
        pMatricesData->View[i] = *(viewMatrix.data() + i);
        pMatricesData->World[i] = *(worldMatrix.data() + i);
    }

    //Unlock the shader-constant buffer.
    pDeviceContext->Unmap(mpMatricesBuffer,0);

    //Update the shader-constant buffer in the vertex shader object.
    pDeviceContext->VSSetConstantBuffers(0,1,&mpMatricesBuffer);
}


}
