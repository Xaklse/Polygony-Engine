
#ifndef POLY_WINDOWS_APPLICATION_H
#define POLY_WINDOWS_APPLICATION_H


#include "Globals.h"


//Header files useful for Windows programming.
#include <windows.h>
#include <windowsx.h>


#include "Poco/Util/IniFileConfiguration.h"


#include "Application.h"


namespace Poly
{


class WinApplication : public Application
{
public:
    WinApplication();
    virtual ~WinApplication();

    void Exit();
    void Run();

    bool WindowEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
        LPARAM secondParam);

    HWND WindowHandle() const { return mWindowHandle; };

private:
    void CleanUp();
    void Initialize();
    void Shutdown();

    /*Handle to the application instance.*/
    HINSTANCE mInstanceHandle;

    /*String which specifies the window class name.*/
    LPCWSTR mWindowClassName;

    /*Handle to the main window.*/
    HWND mWindowHandle;
};


}


#endif //POLY_WINDOWS_APPLICATION_H
