
#include "Renderer.h"


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Render()
{
    //Creates a window with a message box.
    MessageBox(NULL,L"Initializing Renderer...",L"Polygony Engine",MB_ICONEXCLAMATION | MB_OK);

    return true;
}

bool Renderer::Initialize()
{
    return true;
}
