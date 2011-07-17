
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

int System::Run(const std::string& commandLine)
{
    bool result = Initialize();

    if (result)
    {
        MSG message;

        // Initialize the message structure.
        ZeroMemory(&message,sizeof(MSG));

        // Loop until there is a quit message from the window or the user.
        bool loop = true;

        while (loop)
        {
            // Handle the windows messages.
            if (PeekMessage(&message,NULL,0,0,PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }

            // If windows signals to end the application then exit out.
            if (message.message == WM_QUIT)
            {
                loop = false;
            }
            else
            {
                // Otherwise do the frame processing.
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

bool System::Initialize()
{
    bool result;

    mRenderer = new DX11Renderer();
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
