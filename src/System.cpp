
#include "System.h"


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
        mRenderer->Render();

        Shutdown();
    }

    if (!result)
        return EXIT_FAILURE; //This macro means that the application wasn't successful.
    else
        return EXIT_SUCCESS; //This macro means that the application was successful.
}

bool System::Initialize()
{
    bool result = true;

    mRenderer = new Renderer();
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
