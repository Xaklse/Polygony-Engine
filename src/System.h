
#ifndef POLY_SYSTEM_H
#define POLY_SYSTEM_H


//Header files useful for Windows programming.
#include <windows.h>
#include <windowsx.h>


#include "Globals.h"
#include "Renderer.h"
#include "Input.h"


namespace Poly
{


class System : private boost::noncopyable
{
public:
    System();
    virtual ~System();

    void Exit();
    static void Log(const string& message);
    int Run(HINSTANCE instanceHandle,const string& commandLine);

    bool WindowEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
        LPARAM secondParam);

    static System* Get();
    HWND WindowHandle() { return mWindowHandle; };

private:
    bool Initialize();
    void Shutdown();

    Input* mpInput;
    Renderer* mpRenderer;

    /*Handle to the application instance.*/
    HINSTANCE mInstanceHandle;

    /*String which specifies the window class name.*/
    LPCWSTR mWindowClass;

    /*Handle to the main window.*/
    HWND mWindowHandle;

    Poco::Stopwatch mStopWatch;
};


}


/**
 * Receives all input directed at a window.
 * @param windowHandle  unique handle of the window
 * @param intMessage    received message
 * @param firstParam    first extra value
 * @param secondParam   second extra value
 * @return              exit value; 0 means the message has been handled
 */
static LRESULT CALLBACK WndProc(HWND windowHandle,UINT intMessage,
    WPARAM firstParam,LPARAM secondParam)
{
    if (!Poly::System::Get()->WindowEvent(windowHandle,intMessage,
        firstParam,secondParam))
    {
        //Send the event to the default message handler.
        return DefWindowProc(windowHandle,intMessage,firstParam,secondParam);
    }

    return 0;
}


#endif //POLY_SYSTEM_H
