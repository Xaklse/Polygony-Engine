
#ifndef POLY_INPUT_H
#define POLY_INPUT_H


#include "Globals.h"


namespace Poly
{


class System;


class Input
{
public:
    Input(System* pSystem);
    virtual ~Input();

    bool KeyPressEvent(unsigned int key);
    bool KeyReleaseEvent(unsigned int key);

protected:
    System* mpSystem;
};


}


#include "System.h"


#endif //POLY_INPUT_H
