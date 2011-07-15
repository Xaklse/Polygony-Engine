
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

int System::Run()
{
    bool result = Initialize();

    if (result)
    {
        result = mRenderer->Render();

        Shutdown();
    }

    if (!result)
        return EXIT_FAILURE; //Means that the application wasn't successful.
    else
        return EXIT_SUCCESS; //Means that the application was successful.
}

bool System::Initialize()
{
    bool result = true;

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
