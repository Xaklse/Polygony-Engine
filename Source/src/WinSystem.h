
#ifndef POLY_WINDOWS_SYSTEM_H
#define POLY_WINDOWS_SYSTEM_H


#include "Globals.h"


//Header files useful for Windows programming.
#include <windows.h>
#include <windowsx.h>


#include "Poco/AutoPtr.h"
#include "Poco/Util/IniFileConfiguration.h"


#include "System.h"


namespace Poly
{


class WinSystem : public System
{
public:
    WinSystem();
    virtual ~WinSystem();

    void Exit();
    void Run();

    bool WindowEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
        LPARAM secondParam);

    Poco::Util::IniFileConfiguration* ConfigurationFile()
        { return mpConfigurationFile.get(); };
    HWND WindowHandle() const { return mWindowHandle; };

private:
    void CleanUp();
    void Initialize();
    void Shutdown();

    /*Pointer to the main configuration file.*/
    Poco::AutoPtr<Poco::Util::IniFileConfiguration> mpConfigurationFile;

    /*Handle to the application instance.*/
    HINSTANCE mInstanceHandle;

    /*String which specifies the window class name.*/
    LPCWSTR mWindowClassName;

    /*Handle to the main window.*/
    HWND mWindowHandle;
};


}


#endif //POLY_WINDOWS_SYSTEM_H
