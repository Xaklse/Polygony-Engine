
#ifndef PE_DX11_RENDERER_H
#define PE_DX11_RENDERER_H


#include <windows.h> //Includes header files useful for Windows programming.
//#include <d3d11.h> //...


#include "Renderer.h"


class DX11Renderer : public Renderer
{
public:
    DX11Renderer();
    virtual ~DX11Renderer();

    virtual bool Initialize();

    virtual bool Render();
};


#endif //PE_DX11_RENDERER_H
