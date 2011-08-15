
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

bool Renderer::Initialize()
{
    return true;
}

bool Renderer::Render()
{
    return true;
}


}
