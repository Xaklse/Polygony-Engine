
#ifndef PE_SYSTEM_H
#define PE_SYSTEM_H


#include <windows.h> //Includes lots of other header files useful for Windows programming.


#define WIN32_LEAN_AND_MEAN //Speeds up the build process, it reduces the size of the Win32 header
                            //files by excluding some of the less used APIs.


class System
{
public:
    System();
    virtual ~System();

    bool Initialize();
    void Run();
    void Shutdown();
};


#endif //PE_SYSTEM_H
