
#ifndef PE_SYSTEM_H
#define PE_SYSTEM_H


#include <stdlib.h>  //Includes several general purpose functions for dynamic
                     //memory management, random number generation,
                     //communication with the environment, integer arithmetics,
                     //searching, sorting and converting.
#include <string>    //Includes the string class of the Standard Template
                     //Library (STL).

//Header files useful for Windows programming.
#include <windows.h>
#include <windowsx.h>


#include "Renderer.h"


class System
{
public:
    System();
    virtual ~System();

    int Run(HINSTANCE instanceHandle,const std::string& commandLine);

    bool WindowEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
        LPARAM secondParam);

    HWND GetWindowHandle();

private:
    bool Initialize();
    void Shutdown();

    Renderer* mRenderer;

    /*Handle to the application instance.*/
    HINSTANCE mInstanceHandle;

    /*Handle to the main window.*/
    HWND mWindowHandle;

    /*String which specifies the window class name.*/
    LPCWSTR mWindowClass;
};


static System* sSystem;
static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,
    LPARAM lParam)
{
    if (sSystem != NULL && !sSystem->WindowEvent(hWnd,message,wParam,lParam))
    {
        //Send the event to the default message handler.
        return DefWindowProc(hWnd,message,wParam,lParam);
    }

    return 0;
}


#endif //PE_SYSTEM_H
