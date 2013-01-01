
#include "Input.h"


#include "Application.h"


namespace Poly
{


Input::Input()
{
}

Input::~Input()
{
}

bool Input::KeyPressEvent(uint key)
{
    if (key == 27)
    {
        //Esc key.
        Poly::Application::Get()->Exit();
    }

    return true;
}

bool Input::KeyReleaseEvent(uint key)
{
    return true;
}


}
