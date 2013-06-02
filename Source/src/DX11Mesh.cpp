
#include "DX11Mesh.h"


#include "Exception.h"


namespace Poly
{


DX11Mesh::DX11Mesh() : Mesh(),
    mpIndexBuffer(nullptr),
    mpVertexBuffer(nullptr)
{
}

DX11Mesh::~DX11Mesh()
{
    if (mpIndexBuffer != nullptr)
    {
        mpIndexBuffer->Release();
        mpIndexBuffer = nullptr;
    }

    if (mpVertexBuffer != nullptr)
    {
        mpVertexBuffer->Release();
        mpVertexBuffer = nullptr;
    }
}

void DX11Mesh::Initialize(ID3D11Device* pDevice)
{
    //Create a square using the vertex structure.
    Vertex vertices[] =
    {
        {{-0.5f,-0.5f,0.5f},{1.0f,0.0f,0.0f,1.0f}},
        {{-0.5f,0.5f,0.5f},{0.0f,1.0f,0.0f,1.0f}},
        {{0.5f,0.5f,0.5f},{0.0f,0.0f,1.0f,1.0f}},
        {{0.5f,-0.5f,0.5f},{1.0f,1.0f,1.0f,1.0f}}
    };

    //Define each triangle of the vertex structure.
    uint indices[] = {0,1,2,0,2,3};

    mNumIndices = ARRAYSIZE(indices);
    mNumVertices = ARRAYSIZE(vertices);

////////////////////////////////////////////////////////////////////////////////

    D3D11_BUFFER_DESC vertexBufferDescriptor;
    D3D11_SUBRESOURCE_DATA vertexBufferData;

    //Initialize the vertex buffer structure.
    ZeroMemory(&vertexBufferDescriptor,sizeof(D3D11_BUFFER_DESC));

    //Fill in the structure with the needed information.
    vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * mNumVertices;
    vertexBufferDescriptor.CPUAccessFlags = 0;
    vertexBufferDescriptor.MiscFlags = 0;
    vertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;

    //Fill in the related subresource data.
    vertexBufferData.pSysMem = vertices;
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;

    //Create the vertex buffer.
    HRESULT result = pDevice->CreateBuffer(&vertexBufferDescriptor,
        &vertexBufferData,&mpVertexBuffer);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateVertexBuffer)." +
            DEBUG_INFO,result);
    }

////////////////////////////////////////////////////////////////////////////////

    D3D11_BUFFER_DESC indexBufferDescriptor;
    D3D11_SUBRESOURCE_DATA indexBufferData;

    //Initialize the index buffer structure.
    ZeroMemory(&indexBufferDescriptor,sizeof(D3D11_BUFFER_DESC));

    //Fill in the structure with the needed information.
    indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDescriptor.ByteWidth = sizeof(uint) * mNumIndices;
    indexBufferDescriptor.CPUAccessFlags = 0;
    indexBufferDescriptor.MiscFlags = 0;
    indexBufferDescriptor.StructureByteStride = 0;
    indexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;

    //Fill in the related subresource data.
    indexBufferData.pSysMem = indices;
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    //Create the index buffer.
    result = pDevice->CreateBuffer(&indexBufferDescriptor,&indexBufferData,
        &mpIndexBuffer);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateIndexBuffer)." +
            DEBUG_INFO,result);
    }
}

void DX11Mesh::Render(ID3D11DeviceContext* pDeviceContext)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    //Select which vertex buffer to display.
    pDeviceContext->IASetVertexBuffers(0,1,&mpVertexBuffer,&stride,&offset);

    //Activate the relevant index buffer so that it can be rendered.
    pDeviceContext->IASetIndexBuffer(mpIndexBuffer,DXGI_FORMAT_R32_UINT,0);

    //Select which primitive type is used.
    pDeviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Draw the index buffer to the back buffer.
    pDeviceContext->DrawIndexed(6,0,0);
}


}
