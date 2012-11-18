
#include "Renderer.h"


namespace Poly
{


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

/*virtual*/
void Renderer::Initialize(uint width,uint height,bool fullScreen,
    bool verticalSync)
{
}

/*virtual*/
void Renderer::Render()
{
}

/*virtual*/
bool Renderer::WindowResize()
{
    return false;
}


}
