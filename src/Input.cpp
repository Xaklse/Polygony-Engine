
#include "Input.h"


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
    LOG(TO_STRING(key));

    if (key == 27)
        Poly::System::Get()->Exit();

    return true;
}

bool Input::KeyReleaseEvent(uint key)
{
    return true;
}


}
