
#ifndef POLY_WINDOWS_SYSTEM_H
#define POLY_WINDOWS_SYSTEM_H


//Header files useful for Windows programming.
#include <windows.h>
#include <windowsx.h>


#include "System.h"


namespace Poly
{


class WinSystem : public System
{
public:
    WinSystem();
    virtual ~WinSystem();

    virtual void Exit();
    virtual void Run(const string& commandLine);

    virtual bool WindowEvent(HWND windowHandle,UINT intMessage,
        WPARAM firstParam,LPARAM secondParam);

    HWND WindowHandle() const { return mWindowHandle; };

private:
    virtual void Initialize();
    virtual void Shutdown();

    /*Handle to the application instance.*/
    HINSTANCE mInstanceHandle;

    /*String which specifies the window class name.*/
    LPCWSTR mWindowClassName;

    /*Handle to the main window.*/
    HWND mWindowHandle;
};


}


#endif //POLY_WINDOWS_SYSTEM_H
