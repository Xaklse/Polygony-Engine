
#include "DX11Renderer.h"


DX11Renderer::DX11Renderer(System* system) : Renderer(system)
{
    sSystem = system;

    mInstanceHandle = NULL;
    mWindowHandle = NULL;
    mDevice = NULL;
    mDeviceContext = NULL;
    mSwapChain = NULL;
}

DX11Renderer::~DX11Renderer()
{
    if (mSwapChain != NULL)
    {
        mSwapChain->Release();
        mSwapChain = NULL;
    }

    if (mDeviceContext != NULL)
    {
        mDeviceContext->Release();
        mDeviceContext = NULL;
    }

    if (mDevice != NULL)
    {
        mDevice->Release();
        mDevice = NULL;
    }

    if (mWindowHandle != NULL)
    {
        DestroyWindow(mWindowHandle);
        mWindowHandle = NULL;
    }

    if (mInstanceHandle != NULL)
    {
        UnregisterClass(L"PolygonyEngine",mInstanceHandle);
        mInstanceHandle = NULL;
    }

    sSystem = NULL;
}

bool DX11Renderer::Initialize()
{
    mInstanceHandle = GetModuleHandle(NULL);

    WNDCLASSEX windowClass;

    //Initialize the window class structure.
    ZeroMemory(&windowClass,sizeof(WNDCLASSEX));

    //Fill in the structure with the needed information.
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    windowClass.hCursor = LoadCursor(NULL,IDC_ARROW);
    windowClass.hInstance = mInstanceHandle;
    windowClass.lpfnWndProc = WndProc;
    windowClass.lpszClassName = L"PolygonyEngine";
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    //Register the window class.
    RegisterClassEx(&windowClass);

    //Initialize a rectangle with the size of the desired client area.
    RECT windowRect = {0,0,800,600};

    //Adjust the size for the final window.
    AdjustWindowRect(&windowRect,WS_OVERLAPPEDWINDOW,FALSE);

    //Create the window and use the result as the handle.
    mWindowHandle = CreateWindowEx(NULL,L"PolygonyEngine",L"Polygony Engine",
        WS_OVERLAPPEDWINDOW,(GetSystemMetrics(SM_CXSCREEN) - 800) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - 600) / 2,
        windowRect.right - windowRect.left,windowRect.bottom - windowRect.top,
        NULL,NULL,mInstanceHandle,NULL);

    //Display the window on the screen.
    ShowWindow(mWindowHandle,SW_SHOW);

    //Bring the window up on the screen and set it as main focus.
    SetForegroundWindow(mWindowHandle);
    SetFocus(mWindowHandle);

    DXGI_SWAP_CHAIN_DESC swapChainDescriptor;

    //Initialize the window class structure.
    ZeroMemory(&swapChainDescriptor,sizeof(DXGI_SWAP_CHAIN_DESC));

    //Fill in the structure with the needed information.
    swapChainDescriptor.BufferCount = 1;
    swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.OutputWindow = mWindowHandle;
    swapChainDescriptor.SampleDesc.Count = 4;
    swapChainDescriptor.Windowed = TRUE;

    //Create a device, device context and swap chain.
    D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,NULL,NULL,
        NULL,D3D11_SDK_VERSION,&swapChainDescriptor,&mSwapChain,&mDevice,NULL,
        &mDeviceContext);

    return true;
}

bool DX11Renderer::Render()
{
    return true;
}
