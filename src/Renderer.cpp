
#include "Renderer.h"


namespace Poly
{


Renderer::Renderer(System* pSystem)
{
    mpSystem = pSystem;
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(uint width,uint height,bool fullScreen,
    bool verticalSync)
{
    return true;
}

bool Renderer::Render()
{
    return true;
}

bool Renderer::WindowResize()
{
    return false;
}


}
