
#ifndef POLY_DX11_RENDERER_H
#define POLY_DX11_RENDERER_H


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


namespace Poly
{


class DX11Renderer : public Renderer
{
public:
    DX11Renderer(System* system);
    virtual ~DX11Renderer();

    virtual bool Initialize(bool fullScreen,unsigned int width,
        unsigned int height,bool verticalSync);

    virtual bool Render();

private:
    /*The depth buffer stored in a 2D texture.*/
    ID3D11Texture2D* mpDepthBuffer;

    /*The depth-stencil state interface.*/
    ID3D11DepthStencilState* mpDepthStencilState;

    /*The depth-stencil view interface.*/
    ID3D11DepthStencilView* mpDepthStencilView;

    /*A virtual representation of the video adapter.*/
    ID3D11Device* mpDevice;

    /*Used to render graphics and to determine how they will be rendered.*/
    ID3D11DeviceContext* mpDeviceContext;

    /*The rasterizer state.*/
    ID3D11RasterizerState* mpRasterizerState;

    /*The render target view which will store the back buffer pointer.*/
    ID3D11RenderTargetView* mpRenderTarget;

    /*Chain of buffers which swap positions each time a frame is rendered.*/
    IDXGISwapChain* mpSwapChain;

    bool mVerticalSync;
    D3DXMATRIX mOrthoMatrix;
    D3DXMATRIX mProjectionMatrix;
    D3DXMATRIX mWorldMatrix;
};


}


#endif //POLY_DX11_RENDERER_H
