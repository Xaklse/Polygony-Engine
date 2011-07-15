
#include "DX11Renderer.h"


DX11Renderer::DX11Renderer()
{
}

DX11Renderer::~DX11Renderer()
{
}

bool DX11Renderer::Initialize()
{
    return true;
}

bool DX11Renderer::Render()
{
    //Creates a window with a message box.
    MessageBox(NULL,L"Renderer starts rendering...",L"Polygony Engine",
        MB_ICONEXCLAMATION | MB_OK);

    return true;
}
