
#include "DX11Renderer.h"


DX11Renderer::DX11Renderer(System* system) : Renderer(system)
{
    mDevice = NULL;
    mDeviceContext = NULL;
    mSwapChain = NULL;
}

DX11Renderer::~DX11Renderer()
{
    LOG("Shutting down DirectX 11 renderer...");

    if (mSwapChain != NULL)
    {
        mSwapChain->Release();
        mSwapChain = NULL;
    }

    if (mDeviceContext != NULL)
    {
        mDeviceContext->Release();
        mDeviceContext = NULL;
    }

    if (mDevice != NULL)
    {
        mDevice->Release();
        mDevice = NULL;
    }
}

bool DX11Renderer::Initialize()
{
    LOG("Initializing DirectX 11 renderer...");

    DXGI_SWAP_CHAIN_DESC swapChainDescriptor;

    //Initialize the window class structure.
    ZeroMemory(&swapChainDescriptor,sizeof(DXGI_SWAP_CHAIN_DESC));

    //Fill in the structure with the needed information.
    swapChainDescriptor.BufferCount = 1;
    swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.OutputWindow = mSystem->GetWindowHandle();
    swapChainDescriptor.SampleDesc.Count = 4;
    swapChainDescriptor.Windowed = TRUE;

    //Create a device, device context and swap chain.
    if (D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,NULL,
        NULL,NULL,D3D11_SDK_VERSION,&swapChainDescriptor,&mSwapChain,&mDevice,
        NULL,&mDeviceContext) == S_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DX11Renderer::Render()
{
    return true;
}
