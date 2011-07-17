
#ifndef PE_SYSTEM_H
#define PE_SYSTEM_H


#include <stdlib.h>  //Includes several general purpose functions for dynamic
                     //memory management, random number generation,
                     //communication with the environment, integer arithmetics,
                     //searching, sorting and converting.
#include <string>    //Includes the string class of the Standard Template
                     //Library (STL).
#include <windows.h> //Header file useful for Windows programming.


#include "Renderer.h"


class System
{
public:
    System();
    virtual ~System();

    int Run(const std::string& commandLine);

private:
    bool Initialize();
    void Shutdown();

    Renderer* mRenderer;
};


#endif //PE_SYSTEM_H
