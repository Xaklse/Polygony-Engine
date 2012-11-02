
#include "Input.h"


namespace Poly
{


Input::Input(System* pSystem)
{
    mpSystem = pSystem;
}

Input::~Input()
{
}

bool Input::KeyPressEvent(uint key)
{
    LOG(TO_STRING(key));

    if (key == 27)
        mpSystem->Exit();

    return true;
}

bool Input::KeyReleaseEvent(uint key)
{
    return true;
}


}
