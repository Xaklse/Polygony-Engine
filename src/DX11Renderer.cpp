
#include "DX11Renderer.h"


#include "Poco/Util/IniFileConfiguration.h"


#include "Exception.h"


namespace Poly
{


DX11Renderer::DX11Renderer(System* pSystem) : Renderer(pSystem)
{
    mpDevice = NULL;
    mpDeviceContext = NULL;
    mpSwapChain = NULL;
}

DX11Renderer::~DX11Renderer()
{
    LOG("Shutting down DirectX 11 renderer...");

    if (mpSwapChain != NULL)
    {
        //Switch to windowed mode.
        mpSwapChain->SetFullscreenState(false,NULL);

        mpSwapChain->Release();
        mpSwapChain = NULL;
    }

    if (mpRenderTarget != NULL)
    {
        mpRenderTarget->Release();
        mpRenderTarget = NULL;
    }

    if (mpDeviceContext != NULL)
    {
        mpDeviceContext->Release();
        mpDeviceContext = NULL;
    }

    if (mpDevice != NULL)
    {
        mpDevice->Release();
        mpDevice = NULL;
    }
}

bool DX11Renderer::Initialize(bool fullScreen,unsigned int width,
    unsigned int height)
{
    LOG("Initializing DirectX 11 renderer...");

    DXGI_SWAP_CHAIN_DESC swapChainDescriptor;

    //Initialize the swap chain descriptor structure.
    ZeroMemory(&swapChainDescriptor,sizeof(DXGI_SWAP_CHAIN_DESC));

    //Fill in the structure with the needed information.
    swapChainDescriptor.BufferCount = 1;
    swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDescriptor.OutputWindow = mpSystem->GetWindowHandle();
    swapChainDescriptor.SampleDesc.Count = 4;
    swapChainDescriptor.Windowed = fullScreen ? FALSE : TRUE;

    //Create a device, device context and swap chain.
    HRESULT result = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,NULL,NULL,NULL,NULL,D3D11_SDK_VERSION,
        &swapChainDescriptor,&mpSwapChain,&mpDevice,NULL,&mpDeviceContext);

    if (result != S_OK)
    {
        throw Exception("Direct3D 11 init failed (CreateDeviceAndSwapChain).",
            result);
    }

    IDXGIDevice* pDeviceInterface;
    result = mpDevice->QueryInterface(__uuidof(IDXGIDevice),
        (void **)&pDeviceInterface);

    if (result != S_OK)
    {
        throw Exception("Direct3D 11 init failed (QueryInterface).",
            result);
    }

    IDXGIAdapter* pAdapter;
    result = pDeviceInterface->GetParent(__uuidof(IDXGIAdapter),
        (void **)&pAdapter);

    if (result != S_OK)
    {
        throw Exception("Direct3D 11 init failed (GetParent Adapter).",
            result);
    }

    IDXGIFactory* pFactory;
    result = pAdapter->GetParent(__uuidof(IDXGIFactory),(void **)&pFactory);

    if (result != S_OK)
    {
        throw Exception("Direct3D 11 init failed (GetParent Factory).",
            result);
    }

    result = pFactory->MakeWindowAssociation(mpSystem->GetWindowHandle(),
        DXGI_MWA_NO_ALT_ENTER);

    if (result != S_OK)
    {
        throw Exception("Direct3D 11 init failed (MakeWindowAssociation).",
            result);
    }

    pFactory->Release();
    pAdapter->Release();
    pDeviceInterface->Release();

    //Get the pointer to the back buffer.
    ID3D11Texture2D* pTexture;
    result = mpSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),
        (LPVOID*)&pTexture);

    if (result != S_OK)
    {
        throw Exception("Direct3D 11 init failed (GetBuffer).",result);
    }

    //Create the render target view with the back buffer pointer.
    result = mpDevice->CreateRenderTargetView(pTexture,NULL,&mpRenderTarget);

    if (result != S_OK)
    {
        throw Exception("Direct3D 11 init failed (CreateRenderTargetView).",
            result);
    }

    //The pointer to the back buffer is no longer needed.
    pTexture->Release();

    //Set the render target view as the back buffer.
    mpDeviceContext->OMSetRenderTargets(1,&mpRenderTarget,NULL);

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

    return true;
}

bool DX11Renderer::Render()
{
    //Clear the back buffer to a plain color.
    mpDeviceContext->ClearRenderTargetView(mpRenderTarget,
        D3DXCOLOR(0.0f,0.2f,0.4f,1.0f));

    //Switch the front buffer with the back buffer.
    mpSwapChain->Present(0,0);

    return true;
}


}
