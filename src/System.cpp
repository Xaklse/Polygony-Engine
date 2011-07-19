
#include "System.h"


#include "DX11Renderer.h"


System::System()
{
    mInstanceHandle = NULL;
    mRenderer = NULL;
    mWindowHandle = NULL;

    mWindowClass = L"PolygonyEngine";

    sSystem = this;
}

System::~System()
{
    Shutdown();
}

bool System::Initialize()
{
    //Default configuration values.
    bool fullScreen = false;
    unsigned int fullscreenWidth = 1280,fullscreenHeight = 720;
    unsigned int windowWidth = 800,windowHeight = 600;

    unsigned int width = fullScreen ? fullscreenWidth : windowWidth;
    unsigned int height = fullScreen ? fullscreenHeight : windowHeight;

    WNDCLASSEX windowClass;

    //Initialize the window class structure.
    ZeroMemory(&windowClass,sizeof(WNDCLASSEX));

    //Fill in the structure with the needed information.
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    windowClass.hCursor = LoadCursor(NULL,IDC_ARROW);
    windowClass.hInstance = mInstanceHandle;
    windowClass.lpfnWndProc = WndProc;
    windowClass.lpszClassName = mWindowClass;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    //Register the window class.
    RegisterClassEx(&windowClass);

    //Initialize a rectangle with the size of the desired client area.
    RECT windowRect = {0,0,width,height};

    //Adjust the size for the final window.
    AdjustWindowRect(&windowRect,WS_OVERLAPPEDWINDOW,FALSE);

    //Create the window and use the result as the handle.
    mWindowHandle = CreateWindowEx(NULL,mWindowClass,L"Polygony Engine",
        WS_OVERLAPPEDWINDOW,(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
        windowRect.right - windowRect.left,windowRect.bottom - windowRect.top,
        NULL,NULL,mInstanceHandle,NULL);

    if (mWindowHandle != NULL)
    {
        //Display the window on the screen.
        ShowWindow(mWindowHandle,SW_SHOW);

        //Bring the window up on the screen and set it as main focus.
        SetForegroundWindow(mWindowHandle);
        SetFocus(mWindowHandle);

        mRenderer = new DX11Renderer(this);

        return mRenderer->Initialize();
    }
    else
    {
        return false;
    }
}

void System::Shutdown()
{
    if (mRenderer != NULL)
    {
        delete mRenderer;
        mRenderer = NULL;
    }

    if (mWindowHandle != NULL)
    {
        DestroyWindow(mWindowHandle);
        mWindowHandle = NULL;
    }

    if (mInstanceHandle != NULL)
    {
        UnregisterClass(mWindowClass,mInstanceHandle);
        mInstanceHandle = NULL;
    }

    sSystem = NULL;
}

int System::Run(HINSTANCE instanceHandle,const std::string& commandLine)
{
    mInstanceHandle = instanceHandle;

    bool result = Initialize();

    if (result)
    {
        MSG message;

        //Initialize the message structure.
        ZeroMemory(&message,sizeof(MSG));

        bool loop = true;

        //Loop until there is a quit message from the window or the user.
        while (loop)
        {
            //Handle the Windows messages.
            if (PeekMessage(&message,NULL,0,0,PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }

            //If Windows signals to end the application then exit out.
            if (message.message == WM_QUIT)
            {
                loop = false;
            }
            else
            {
                //Otherwise do the frame processing.
                result = mRenderer->Render();

                if (!result)
                {
                    loop = false;
                }
            }

        }

        Shutdown();
    }

    if (!result)
        return EXIT_FAILURE; //Means that the application wasn't successful.
    else
        return EXIT_SUCCESS; //Means that the application was successful.
}

bool System::WindowEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
    LPARAM secondParam)
{
    switch (intMessage)
    {
        //Check if the window is being destroyed.
        case WM_DESTROY:
            PostQuitMessage(0);

            break;

        //Check if the window is being closed.
        case WM_CLOSE:
            PostQuitMessage(0);

            break;

        //Check if a key has been pressed on the keyboard.
        case WM_KEYDOWN:
            //If a key is pressed send it to the input object so it can record that state.
//            m_Input->KeyDown((unsigned int)wparam);
            break;

        //Check if a key has been released on the keyboard.
        case WM_KEYUP:
            //If a key is released then send it to the input object so it can unset the state for that key.
//            m_Input->KeyUp((unsigned int)wparam);
            break;

        //Any other messages send to the default message handler as our application won't make use of them.
        default:
            return false;
    }

    return true;
}

HWND System::GetWindowHandle()
{
    return mWindowHandle;
}
