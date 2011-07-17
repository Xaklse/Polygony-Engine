
#include "DX11Renderer.h"


DX11Renderer::DX11Renderer()
{
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
}

bool DX11Renderer::Initialize()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc,sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    // calculate the size of the client area
    RECT wr = {0,0,500,400}; // set the size, but not the position
    AdjustWindowRect(&wr,WS_OVERLAPPEDWINDOW,FALSE); // adjust the size

    // create the window and use the result as the handle
    HWND hWnd = CreateWindowEx(NULL,
                          L"WindowClass1",    // name of the window class
                          L"Our First Windowed Program",   // title of the window
                          WS_OVERLAPPEDWINDOW,    // window style
                          300,    // x-position of the window
                          300,    // y-position of the window
                          wr.right - wr.left,    // width of the window
                          wr.bottom - wr.top,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(hWnd,SW_SHOW);

    // bring the window up on the screen and set it as main focus
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd,sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &mSwapChain,
                                  &mDevice,
                                  NULL,
                                  &mDeviceContext);

    return true;
}

bool DX11Renderer::Render()
{
    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    switch (message)
    {
        // Check if the window is being destroyed.
        case WM_DESTROY:
            PostQuitMessage(0);

            break;

        // Check if the window is being closed.
        case WM_CLOSE:
            PostQuitMessage(0);

            break;

        // Check if a key has been pressed on the keyboard.
        case WM_KEYDOWN:
            // If a key is pressed send it to the input object so it can record that state.
//            m_Input->KeyDown((unsigned int)wparam);
            break;

        // Check if a key has been released on the keyboard.
        case WM_KEYUP:
            // If a key is released then send it to the input object so it can unset the state for that key.
//            m_Input->KeyUp((unsigned int)wparam);
            break;

        // Any other messages send to the default message handler as our application won't make use of them.
        default:
            return DefWindowProc(hWnd,message,wParam,lParam);
    }

    return 0;
}
