
#ifndef PE_SYSTEM_H
#define PE_SYSTEM_H


#define WIN32_LEAN_AND_MEAN //Speeds up the build process, it reduces the size
                            //of the Win32 header files by excluding some of the
                            //less used APIs.


#include <stdlib.h>  //Includes several general purpose functions for dynamic
                     //memory management, random number generation,
                     //communication with the environment, integer arithmetics,
                     //searching, sorting and converting.
#include <windows.h> //Includes header files useful for Windows programming.


#include "Renderer.h"


class System
{
public:
    System();
    virtual ~System();

    int Run();

private:
    bool Initialize();
    void Shutdown();

    Renderer* mRenderer;
};


#endif //PE_SYSTEM_H
