
#include "DX11Renderer.h"


DX11Renderer::DX11Renderer()
{
    mDevice = NULL;
    mDeviceContext = NULL;
    mSwapChain = NULL;
}

DX11Renderer::~DX11Renderer()
{
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
    //@todo: fill with DX11 stuff.

    return true;
}

bool DX11Renderer::Render()
{
    //Creates a window with a message box.
    MessageBox(NULL,L"Renderer starts rendering...",L"Polygony Engine",
        MB_ICONEXCLAMATION | MB_OK);

    return true;
}
