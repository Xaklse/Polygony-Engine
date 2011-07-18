
#include "Renderer.h"


Renderer::Renderer(System* system)
{
    mSystem = system;
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
