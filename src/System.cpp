
#include "System.h"


#include "DX11Renderer.h"


System::System()
{
    mRenderer = NULL;
}

System::~System()
{
    Shutdown();
}

bool System::Initialize()
{
    bool result;

    mRenderer = new DX11Renderer(this);
    result = mRenderer->Initialize();

    return result;
}

void System::Shutdown()
{
    if (mRenderer != NULL)
    {
        delete mRenderer;
        mRenderer = NULL;
    }
}

int System::Run(const std::string& commandLine)
{
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

bool System::WindowsEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
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
