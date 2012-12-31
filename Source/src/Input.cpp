
#include "Input.h"


#include "System.h"


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
        Poly::System::Get()->Exit();
    }

    return true;
}

bool Input::KeyReleaseEvent(uint key)
{
    return true;
}


}
