
#ifndef POLY_SYSTEM_H
#define POLY_SYSTEM_H


#include <iostream> //Provides input and output functionality using streams. A
                    //stream is an abstraction that represents a device on which
                    //input and ouput operations are performed.
#include <stdlib.h> //Includes several general purpose functions for dynamic
                    //memory management, random number generation,
                    //communication with the environment, integer arithmetics,
                    //searching, sorting and converting.
#include <string>   //Includes the string class of the Standard Template
                    //Library (STL).

//Header files useful for Windows programming.
#include <windows.h>
#include <windowsx.h>


#include "boost/lexical_cast.hpp"
#include "Poco/Logger.h"
#include "Poco/String.h"
#include "Poco/StopWatch.h"


#include "Globals.h"
#include "Renderer.h"
#include "Input.h"


namespace Poly
{


class System
{
public:
    System();
    virtual ~System();

    void Exit();
    void Log(const string& message);
    int Run(HINSTANCE instanceHandle,const string& commandLine);

    bool WindowEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
        LPARAM secondParam);

    HWND GetWindowHandle();

private:
    bool Initialize();
    void Shutdown();

    Input* mpInput;
    Renderer* mpRenderer;
    System* mpSystem;

    /*Handle to the application instance.*/
    HINSTANCE mInstanceHandle;

    /*Handle to the main window.*/
    HWND mWindowHandle;

    /*String which specifies the window class name.*/
    LPCWSTR mWindowClass;

    Poco::Stopwatch mStopWatch;
};


}


static Poly::System* spSystem;
static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,
    LPARAM lParam)
{
    if (spSystem != NULL && !spSystem->WindowEvent(hWnd,message,wParam,lParam))
    {
        //Send the event to the default message handler.
        return DefWindowProc(hWnd,message,wParam,lParam);
    }

    return 0;
}


#endif //POLY_SYSTEM_H
