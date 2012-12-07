
#include "DX11Renderer.h"


#include "Poco/Util/IniFileConfiguration.h"


#include "Exception.h"
#include "WinSystem.h"


namespace Poly
{


DX11Renderer::DX11Renderer() : Renderer(),
    mpBackBufferRenderTarget(nullptr),
    mpDepthBufferTexture(nullptr),
    mpDepthStencilState(nullptr),
    mpDepthStencilView(nullptr),
    mpDevice(nullptr),
    mpDeviceContext(nullptr),
    mpRasterizerState(nullptr),
    mpSwapChain(nullptr),

    mpInputLayout(nullptr),
    mpPixelShader(nullptr),
    mpVertexBuffer(nullptr),
    mpVertexShader(nullptr)
{
}

DX11Renderer::~DX11Renderer()
{
    LOG("Shutting down DirectX 11 renderer...");

    if (mpInputLayout != nullptr)
    {
        mpInputLayout->Release();
        mpInputLayout = nullptr;
    }

    if (mpPixelShader != nullptr)
    {
        mpPixelShader->Release();
        mpPixelShader = nullptr;
    }

    if (mpVertexShader != nullptr)
    {
        mpVertexShader->Release();
        mpVertexShader = nullptr;
    }

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

    if (mpSwapChain != nullptr)
    {
        //Switch to windowed mode.
        mpSwapChain->SetFullscreenState(false,nullptr);

        mpSwapChain->Release();
        mpSwapChain = nullptr;
    }

    if (mpRasterizerState != nullptr)
    {
        mpRasterizerState->Release();
        mpRasterizerState = nullptr;
    }

    if (mpDepthStencilView != nullptr)
    {
        mpDepthStencilView->Release();
        mpDepthStencilView = nullptr;
    }

    if (mpDepthStencilState != nullptr)
    {
        mpDepthStencilState->Release();
        mpDepthStencilState = nullptr;
    }

    if (mpDepthBufferTexture != nullptr)
    {
        mpDepthBufferTexture->Release();
        mpDepthBufferTexture = nullptr;
    }

    if (mpBackBufferRenderTarget != nullptr)
    {
        mpBackBufferRenderTarget->Release();
        mpBackBufferRenderTarget = nullptr;
    }

    if (mpDeviceContext != nullptr)
    {
        mpDeviceContext->Release();
        mpDeviceContext = nullptr;
    }

    if (mpDevice != nullptr)
    {
        mpDevice->Release();
        mpDevice = nullptr;
    }
}

/*virtual*/
void DX11Renderer::Initialize(uint width,uint height,bool fullScreen)
{
    LOG("Initializing DirectX 11 renderer...");

    mVerticalSync = static_cast<WinSystem*>(Poly::System::Get())->
        ConfigurationFile()->getBool("Renderer.VerticalSync",false);

////////////////////////////////////////////////////////////////////////////////

    DXGI_SWAP_CHAIN_DESC swapChainDescriptor;

    //Initialize the swap chain descriptor structure.
    ZeroMemory(&swapChainDescriptor,sizeof(DXGI_SWAP_CHAIN_DESC));

    //Fill in the structure with the needed information.
    swapChainDescriptor.BufferCount = 1;
    swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDescriptor.BufferDesc.ScanlineOrdering =
        DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDescriptor.OutputWindow = static_cast<Poly::WinSystem*>(
        Poly::System::Get())->WindowHandle();
    swapChainDescriptor.SampleDesc.Count = 1;
    swapChainDescriptor.SampleDesc.Quality = 0;
    swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDescriptor.Windowed = fullScreen ? FALSE : TRUE;

    if (!mVerticalSync)
    {
        swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 1;
    }
    else
    {
        swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 1;
    }

    //Create a device, device context and swap chain.
    HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr,
        D3D_DRIVER_TYPE_HARDWARE,nullptr,0,nullptr,0,D3D11_SDK_VERSION,
        &swapChainDescriptor,&mpSwapChain,&mpDevice,nullptr,&mpDeviceContext);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateDeviceAndSwapChain)." +
            DEBUG_INFO,result);
    }

////////////////////////////////////////////////////////////////////////////////

    //Get a pointer to the DXGI device interface.
    IDXGIDevice* pDeviceInterface;
    result = mpDevice->QueryInterface(__uuidof(IDXGIDevice),
        (void**)&pDeviceInterface);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (QueryInterface)." +
            DEBUG_INFO,result);
    }

    //Get a pointer to the DXGI adapter interface which often represents a video
    //card.
    IDXGIAdapter* pAdapter;
    result = pDeviceInterface->GetParent(__uuidof(IDXGIAdapter),
        (void**)&pAdapter);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (GetParent Adapter)." +
            DEBUG_INFO,result);
    }

    //Get a pointer to the DXGI factory interface which can generate other DXGI
    //objects and handle full-screen transitions.
    IDXGIFactory* pFactory;
    result = pAdapter->GetParent(__uuidof(IDXGIFactory),(void**)&pFactory);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (GetParent Factory)." +
            DEBUG_INFO,result);
    }

    //Ignore full-screen transitions through the Alt+Enter key combination.
    result = pFactory->MakeWindowAssociation(
        static_cast<Poly::WinSystem*>(Poly::System::Get())->WindowHandle(),
        DXGI_MWA_NO_ALT_ENTER);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (MakeWindowAssociation)." +
            DEBUG_INFO,result);
    }

    //The pointers are no longer needed.
    pFactory->Release();
    pAdapter->Release();
    pDeviceInterface->Release();

////////////////////////////////////////////////////////////////////////////////

    //Get a pointer to the back buffer.
    ID3D11Texture2D* pTexture;
    result = mpSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),
        (LPVOID*)&pTexture);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (GetBuffer)." + DEBUG_INFO,
            result);
    }

    //Create the render target view with the back buffer pointer.
    result = mpDevice->CreateRenderTargetView(pTexture,nullptr,
        &mpBackBufferRenderTarget);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateRenderTargetView)." +
            DEBUG_INFO,result);
    }

    //The pointer to the back buffer is no longer needed.
    pTexture->Release();

////////////////////////////////////////////////////////////////////////////////

    D3D11_TEXTURE2D_DESC textureDescriptor;

    //Initialize the description of the depth buffer.
    ZeroMemory(&textureDescriptor,sizeof(D3D11_TEXTURE2D_DESC));

    //Fill in the structure with the needed information.
    textureDescriptor.ArraySize = 1;
    textureDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDescriptor.CPUAccessFlags = 0;
    textureDescriptor.Height = height;
    textureDescriptor.MipLevels = 1;
    textureDescriptor.MiscFlags = 0;
    textureDescriptor.Width = width;
    textureDescriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDescriptor.SampleDesc.Count = 1;
    textureDescriptor.SampleDesc.Quality = 0;
    textureDescriptor.Usage = D3D11_USAGE_DEFAULT;

    //Create the texture for the depth buffer.
    result = mpDevice->CreateTexture2D(&textureDescriptor,nullptr,
        &mpDepthBufferTexture);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateTexture2D Depth)." +
            DEBUG_INFO,result);
    }

////////////////////////////////////////////////////////////////////////////////

    D3D11_DEPTH_STENCIL_DESC depthStencilDescriptor;

    //Initialize the description of the depth-stencil state interface.
    ZeroMemory(&depthStencilDescriptor,sizeof(D3D11_DEPTH_STENCIL_DESC));

    //Fill in the structure with the needed information.
    depthStencilDescriptor.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDescriptor.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDescriptor.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDescriptor.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDescriptor.DepthEnable = TRUE;
    depthStencilDescriptor.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDescriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDescriptor.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDescriptor.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDescriptor.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDescriptor.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDescriptor.StencilEnable = TRUE;
    depthStencilDescriptor.StencilReadMask = 0xFF;
    depthStencilDescriptor.StencilWriteMask = 0xFF;

    //Create the depth-stencil state interface.
    result = mpDevice->CreateDepthStencilState(&depthStencilDescriptor,
        &mpDepthStencilState);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateDepthStencilState)." +
            DEBUG_INFO,result);
    }

    //Set the depth-stencil state of the output-merger stage.
    mpDeviceContext->OMSetDepthStencilState(mpDepthStencilState,1);

////////////////////////////////////////////////////////////////////////////////

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor;

    //Initialize the description of the depth-stencil view interface.
    ZeroMemory(&depthStencilViewDescriptor,
        sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

    //Fill in the structure with the needed information.
    depthStencilViewDescriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDescriptor.Texture2D.MipSlice = 0;
    depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    //Create the depth-stencil view interface.
    result = mpDevice->CreateDepthStencilView(mpDepthBufferTexture,
        &depthStencilViewDescriptor,&mpDepthStencilView);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateDepthStencilView)." +
            DEBUG_INFO,result);
    }

    //Bind the render target view and depth-stencil view interface to the output
    //render pipeline.
    mpDeviceContext->OMSetRenderTargets(1,&mpBackBufferRenderTarget,
        mpDepthStencilView);

////////////////////////////////////////////////////////////////////////////////

    D3D11_RASTERIZER_DESC rasterizerDescriptor;

    //Initialize the description of the rasterizer state.
    ZeroMemory(&rasterizerDescriptor,sizeof(D3D11_RASTERIZER_DESC));

    //Fill in the structure with the needed information.
    rasterizerDescriptor.AntialiasedLineEnable = FALSE;
    rasterizerDescriptor.CullMode = D3D11_CULL_BACK;
    rasterizerDescriptor.DepthBias = 0;
    rasterizerDescriptor.DepthBiasClamp = 0.0f;
    rasterizerDescriptor.DepthClipEnable = TRUE;
    rasterizerDescriptor.FillMode = D3D11_FILL_SOLID;
    rasterizerDescriptor.FrontCounterClockwise = FALSE;
    rasterizerDescriptor.MultisampleEnable = FALSE;
    rasterizerDescriptor.ScissorEnable = FALSE;
    rasterizerDescriptor.SlopeScaledDepthBias = 0.0f;

    //Create the rasterizer state.
    result = mpDevice->CreateRasterizerState(&rasterizerDescriptor,
        &mpRasterizerState);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateRasterizerState)." +
            DEBUG_INFO,result);
    }

    //Set the rasterizer state.
    mpDeviceContext->RSSetState(mpRasterizerState);

////////////////////////////////////////////////////////////////////////////////

    D3D11_VIEWPORT viewport;

    //Initialize the viewport structure.
    ZeroMemory(&viewport,sizeof(D3D11_VIEWPORT));

    //Fill in the structure with the needed information.
    viewport.Height = static_cast<float>(height);
    viewport.MaxDepth = 1.0f;
    viewport.MinDepth = 0.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);

    //Activate the viewport.
    mpDeviceContext->RSSetViewports(1,&viewport);

////////////////////////////////////////////////////////////////////////////////

    //Create the projection matrix for 3D rendering; uses a left-handed
    //coordinate system.
    D3DXMatrixPerspectiveFovLH(&mProjectionMatrix,
        boost::math::float_constants::pi * 0.25f,
        static_cast<float>(width) / static_cast<float>(height),0.1f,1000.0f);

    //Initialize the world matrix to the identity matrix.
    D3DXMatrixIdentity(&mWorldMatrix);

    //Create an orthographic projection matrix for 2D rendering; uses a
    //left-handed coordinate system.
    D3DXMatrixOrthoLH(&mOrthoMatrix,static_cast<float>(width),
        static_cast<float>(height),0.1f,1000.0f);

////////////////////////////////////////////////////////////////////////////////

    //Create a square using the vertex structure.
    Vertex vertices[] =
    {
        {{-0.5f,-0.5f,0.5f},{1.0f,0.0f,0.0f,1.0f}},
        {{-0.5f,0.5f,0.5f},{0.0f,1.0f,0.0f,1.0f}},
        {{0.5f,0.5f,0.5f},{0.0f,0.0f,1.0f,1.0f}},
        {{0.5f,-0.5f,0.5f},{1.0f,1.0f,1.0f,1.0f}}
    };

    uint numVertices = ARRAYSIZE(vertices);

    //Define each triangle of the vertex structure.
    uint indices[] = {0,1,2,0,2,3};

    uint numIndices = ARRAYSIZE(indices);

////////////////////////////////////////////////////////////////////////////////

    D3D11_BUFFER_DESC vertexBufferDescriptor;
    D3D11_SUBRESOURCE_DATA vertexBufferData;

    //Initialize the vertex buffer structure.
    ZeroMemory(&vertexBufferDescriptor,sizeof(D3D11_BUFFER_DESC));

    //Fill in the structure with the needed information.
    vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * numVertices;
    vertexBufferDescriptor.CPUAccessFlags = 0;
    vertexBufferDescriptor.MiscFlags = 0;
    vertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;

    //Fill in the related subresource data.
    vertexBufferData.pSysMem = vertices;
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;

    //Create the vertex buffer.
    result = mpDevice->CreateBuffer(&vertexBufferDescriptor,&vertexBufferData,
        &mpVertexBuffer);

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
    indexBufferDescriptor.ByteWidth = sizeof(uint) * numIndices;
    indexBufferDescriptor.CPUAccessFlags = 0;
    indexBufferDescriptor.MiscFlags = 0;
    indexBufferDescriptor.StructureByteStride = 0;
    indexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;

    //Fill in the related subresource data.
    indexBufferData.pSysMem = indices;
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    //Create the index buffer.
    result = mpDevice->CreateBuffer(&indexBufferDescriptor,&indexBufferData,
        &mpIndexBuffer);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateIndexBuffer)." +
            DEBUG_INFO,result);
    }

////////////////////////////////////////////////////////////////////////////////

    //Interfaces used to return arbitrary length data.
    ID3D10Blob* pVertexShaderBlob;
    ID3D10Blob* pPixelShaderBlob;

    //Load and compile the vertex shader.
    result = D3DX11CompileFromFile(L"shaders.hlsl",nullptr,nullptr,"VShader",
        "vs_5_0",0,0,nullptr,&pVertexShaderBlob,nullptr,nullptr);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CompileVertexShader)." +
            DEBUG_INFO,result);
    }

    //Load and compile the pixel shader.
    result = D3DX11CompileFromFile(L"shaders.hlsl",nullptr,nullptr,"PShader",
        "ps_5_0",0,0,nullptr,&pPixelShaderBlob,nullptr,nullptr);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CompilePixelShader)." +
            DEBUG_INFO,result);
    }

    //Encapsulate the vertex shader into a shader object.
    result = mpDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(),
        pVertexShaderBlob->GetBufferSize(),nullptr,&mpVertexShader);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateVertexShader)." +
            DEBUG_INFO,result);
    }

    //Encapsulate the pixel shader into a shader object.
    result = mpDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(),
        pPixelShaderBlob->GetBufferSize(),nullptr,&mpPixelShader);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreatePixelShader)." +
            DEBUG_INFO,result);
    }

    //Set the vertex shader object as active and ready for the device.
    mpDeviceContext->VSSetShader(mpVertexShader,nullptr,0);

    //Set the pixel shader object as active and ready for the device.
    mpDeviceContext->PSSetShader(mpPixelShader,nullptr,0);

////////////////////////////////////////////////////////////////////////////////

    //Initialize the descriptor of the input-layout object.
    D3D11_INPUT_ELEMENT_DESC inputElementDescriptor[] =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
         D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
         D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
    };

    //Create the input-layout object.
    result = mpDevice->CreateInputLayout(inputElementDescriptor,2,
        pVertexShaderBlob->GetBufferPointer(),
        pVertexShaderBlob->GetBufferSize(),&mpInputLayout);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (CreateInputLayout)." +
            DEBUG_INFO,result);
    }

    //Bind the input-layout object to the input-assembler stage.
    mpDeviceContext->IASetInputLayout(mpInputLayout);
}

/*virtual*/
void DX11Renderer::Render()
{
    //Clear the back buffer to plain black color.
    mpDeviceContext->ClearRenderTargetView(mpBackBufferRenderTarget,
        D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));

    //Clear the depth buffer.
    mpDeviceContext->ClearDepthStencilView(mpDepthStencilView,D3D11_CLEAR_DEPTH,
        1.0f,0);

////////////////////////////////////////////////////////////////////////////////

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    //Select which vertex buffer to display.
    mpDeviceContext->IASetVertexBuffers(0,1,&mpVertexBuffer,&stride,&offset);

    //Activate the relevant index buffer so that it can be rendered.
    mpDeviceContext->IASetIndexBuffer(mpIndexBuffer,DXGI_FORMAT_R32_UINT,0);

    //Select which primitive type is used.
    mpDeviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Draw the index buffer to the back buffer.
    mpDeviceContext->DrawIndexed(6,0,0);

////////////////////////////////////////////////////////////////////////////////

    if (!mVerticalSync)
    {
        //Switch the front buffer with the back buffer as fast as possible.
        mpSwapChain->Present(0,0);
    }
    else
    {
        //Switch the front buffer with the back buffer.
        mpSwapChain->Present(1,0);
    }

////////////////////////////////////////////////////////////////////////////////

    Renderer::Render();
}

/*virtual*/
bool DX11Renderer::WindowResize()
{
    return false;
}


}
