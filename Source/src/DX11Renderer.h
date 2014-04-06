
#ifndef POLY_DX11_RENDERER_H
#define POLY_DX11_RENDERER_H


#include "Globals.h"
#include "SceneCamera.h"
#include "DX11.h"
#include "DX11PixelShader.h"
#include "DX11VertexShader.h"
#include "DX11Mesh.h"
#include "Renderer.h"


namespace Poly
{


class DX11Renderer : public Renderer
{
public:
    DX11Renderer();
    virtual ~DX11Renderer();

    void Initialize(uint width,uint height,bool fullScreen);

    bool FrameResize();
    void Render();

private:
    void ComRelease(IUnknown* pComInterface);

    /*The render target view which will store the back buffer pointer.*/
    ID3D11RenderTargetView* mpBackBufferRenderTarget;

    /*The depth buffer stored in a 2D texture.*/
    ID3D11Texture2D* mpDepthBufferTexture;

    /*The depth-stencil state interface.*/
    ID3D11DepthStencilState* mpDepthStencilState;

    /*The depth-stencil view interface.*/
    ID3D11DepthStencilView* mpDepthStencilView;

    /*A virtual representation of the video adapter.*/
    ID3D11Device* mpDevice;

    /*Used to render graphics and to determine how they will be rendered.*/
    ID3D11DeviceContext* mpDeviceContext;

    /*The rasterizer state interface.*/
    ID3D11RasterizerState* mpRasterizerState;

    /*Chain of buffers which swap positions each time a frame is rendered.*/
    IDXGISwapChain* mpSwapChain;

    std::unique_ptr<SceneCamera> mpCamera;

    std::unique_ptr<DX11Mesh> mpMesh;

    std::unique_ptr<DX11PixelShader> mpPixelShader;
    std::unique_ptr<DX11VertexShader> mpVertexShader;
};


}


#endif //POLY_DX11_RENDERER_H
