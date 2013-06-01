
#ifndef POLY_DX11_VERTEX_SHADER_H
#define POLY_DX11_VERTEX_SHADER_H


#include "Globals.h"


//Header files which contain all the Direct3D functionality for setting up and
//drawing 3D graphics in DirectX as well as tools to interface with hardware.
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

//Add the Direct3D library files as linker input for the current project.
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dx10.lib")


namespace Poly
{


class DX11VertexShader
{
public:
    DX11VertexShader();
    virtual ~DX11VertexShader();

    void Initialize(const string& shaderFileName,ID3D11Device* pDevice);

    void Render(ID3D11DeviceContext* pDeviceContext);

private:
    ID3D11InputLayout* mpInputLayout;
    ID3D11VertexShader* mpVertexShader;
};


}


#endif //POLY_DX11_VERTEX_SHADER_H
