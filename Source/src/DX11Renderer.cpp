
#include "DX11Renderer.h"


#include "Poco/Util/IniFileConfiguration.h"


#include "Exception.h"
#include "WinApplication.h"


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
    mpSwapChain(nullptr)
{
}

DX11Renderer::~DX11Renderer()
{
    LOG("Shutting down DirectX 11 renderer...");

    if (mpSwapChain != nullptr)
    {
        //Switch to windowed mode.
        mpSwapChain->SetFullscreenState(false,nullptr);
    }

    if (mpVertexShader.get() != nullptr)
    {
        mpVertexShader.reset(nullptr);
    }

    if (mpPixelShader != nullptr)
    {
        mpPixelShader.reset(nullptr);
    }

    if (mpMesh.get() != nullptr)
    {
        mpMesh.reset(nullptr);
    }

    if (mpCamera.get() != nullptr)
    {
        mpCamera.reset(nullptr);
    }

    ComRelease(mpSwapChain);
    ComRelease(mpRasterizerState);
    ComRelease(mpDepthStencilView);
    ComRelease(mpDepthStencilState);
    ComRelease(mpDepthBufferTexture);
    ComRelease(mpBackBufferRenderTarget);
    ComRelease(mpDeviceContext);
    ComRelease(mpDevice);
}

void DX11Renderer::ComRelease(IUnknown* pComInterface)
{
    if (pComInterface != nullptr)
    {
        pComInterface->Release();
        pComInterface = nullptr;
    }
}

bool DX11Renderer::FrameResize()
{
    return false;
}

void DX11Renderer::Initialize(uint width,uint height,bool fullScreen)
{
    LOG("Initializing DirectX 11 renderer...");

    Renderer::Initialize(width,height,fullScreen);

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
    swapChainDescriptor.OutputWindow = static_cast<Poly::WinApplication*>(
        Poly::Application::Get())->WindowHandle();
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
        static_cast<Poly::WinApplication*>(Poly::Application::Get())->
        WindowHandle(),DXGI_MWA_NO_ALT_ENTER);

    if (FAILED(result))
    {
        throw Exception("Direct3D 11 init failed (MakeWindowAssociation)." +
            DEBUG_INFO,result);
    }

    //These pointers are no longer needed.
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

    //This pointer is no longer needed.
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

    //Create an orthographic projection matrix for 2D rendering; uses a
    //left-handed coordinate system.
    CalculateOrthographicMatrix(static_cast<float>(width),
        static_cast<float>(height),0.1f,1000.0f);

    //Create the perspective projection matrix based on a field of view for 3D
    //rendering; uses a left-handed coordinate system.
    CalculatePerspectiveMatrix(static_cast<float>(width),
        static_cast<float>(height),0.1f,1000.0f,PI * 0.25f);

    //Initialize the world-space matrix to the identity matrix.
    mWorldMatrix << Matrix4::Identity();

////////////////////////////////////////////////////////////////////////////////

    //Create the main scene camera.
    mpCamera.reset(NEW SceneCamera());

////////////////////////////////////////////////////////////////////////////////

    //Create and initialize a mesh.
    mpMesh.reset(NEW DX11Mesh());

    mpMesh->Initialize(mpDevice);

////////////////////////////////////////////////////////////////////////////////

    //Create and initialize the needed shaders.
    mpPixelShader.reset(NEW DX11PixelShader());
    mpVertexShader.reset(NEW DX11VertexShader());

    mpPixelShader->Initialize("pixelshader.hlsl",mpDevice);
    mpVertexShader->Initialize("vertexshader.hlsl",mpDevice);
}

void DX11Renderer::Render()
{
    //Background color (solid black).
    float clearColor[4] = {0.0f,0.0f,0.0f,1.0f};

    //Clear the back buffer to previous color.
    mpDeviceContext->ClearRenderTargetView(mpBackBufferRenderTarget,clearColor);

    //Clear the depth buffer.
    mpDeviceContext->ClearDepthStencilView(mpDepthStencilView,D3D11_CLEAR_DEPTH,
        1.0f,0);

////////////////////////////////////////////////////////////////////////////////

    //Generate the view matrix and set up the shaders.
    mpVertexShader->Render(mpDeviceContext,mWorldMatrix,mpCamera->Render(),
        mPerspectiveMatrix);
    mpPixelShader->Render(mpDeviceContext);

////////////////////////////////////////////////////////////////////////////////

    //Draw a mesh.
    mpMesh->Render(mpDeviceContext);

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


}
