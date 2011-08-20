
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

bool Renderer::Initialize(bool fullScreen,unsigned int width,
    unsigned int height)
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
