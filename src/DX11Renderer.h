
#ifndef PE_DX11_RENDERER_H
#define PE_DX11_RENDERER_H


#include <windows.h> //Header file useful for Windows programming.

//Header files which contain all the Direct3D functionality for setting up and
//drawing 3D graphics in DirectX as well as tools to interface with hardware.
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

//Add the Direct3D library files as linker input for the current project.
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dx10.lib")


#include "Renderer.h"


class DX11Renderer : public Renderer
{
public:
    DX11Renderer(System* system);
    virtual ~DX11Renderer();

    virtual bool Initialize();

    virtual bool Render();

private:
    /*A virtual representation of the video adapter.*/
    ID3D11Device* mDevice;

    /*Used to render graphics and to determine how they will be rendered.*/
    ID3D11DeviceContext* mDeviceContext;

    /*Chain of buffers which swap positions each time a frame is rendered.*/
    IDXGISwapChain* mSwapChain;
};


#endif //PE_DX11_RENDERER_H
