
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

bool Input::KeyPressEvent(unsigned int key)
{
    LOG(TO_STRING(key));

    if (key == 27)
        mpSystem->Exit();

    return true;
}

bool Input::KeyReleaseEvent(unsigned int key)
{
    return true;
}


}
