
#include <System.h>


System::System()
{
}

System::~System()
{
}

bool System::Initialize()
{
    //Create a "Hello World" message box.
    MessageBox(NULL,L"Hello World!",L"Another Hello World program!",MB_ICONEXCLAMATION | MB_OK);

    return EXIT_SUCCESS; //This macro means that the application was successful.
}

void System::Run()
{
}

void System::Shutdown()
{
}
