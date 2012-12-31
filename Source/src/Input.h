
#ifndef POLY_INPUT_H
#define POLY_INPUT_H


#include "Globals.h"


namespace Poly
{


class Input : private boost::noncopyable
{
public:
    Input();
    virtual ~Input();

    bool KeyPressEvent(uint key);
    bool KeyReleaseEvent(uint key);
};


}


#endif //POLY_INPUT_H
