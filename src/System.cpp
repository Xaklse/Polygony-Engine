
#include "System.h"


#include "Poco/AutoPtr.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/FileChannel.h"
#include "Poco/LocalDateTime.h"
#include "Poco/SplitterChannel.h"
#include "Poco/WindowsConsoleChannel.h"

#include "Poco/Util/IniFileConfiguration.h"


#include "DX11Renderer.h"
#include "Exception.h"


namespace Poly
{


System::System()
{
    mInstanceHandle = NULL;
    mpInput = NULL;
    mpRenderer = NULL;
    mWindowHandle = NULL;

    mWindowClass = L"PolygonyEngine";

    spSystem = this;
}

System::~System()
{
    Shutdown();
}

void System::Exit()
{
    DestroyWindow(mWindowHandle);
}

bool System::Initialize()
{
    try
    {
        mStopWatch.start();

        bool result = AllocConsole() != 0;

        Poco::AutoPtr<Poco::WindowsConsoleChannel> pConsoleChannel(
            new Poco::WindowsConsoleChannel());

        Poco::AutoPtr<Poco::FileChannel> pFileChannel(new Poco::FileChannel());
        pFileChannel->setProperty("path","log.log");
        pFileChannel->setProperty("rotation","10 M");

        Poco::AutoPtr<Poco::SplitterChannel> pSplitterChannel(
            new Poco::SplitterChannel());
        pSplitterChannel->addChannel(pConsoleChannel);
        pSplitterChannel->addChannel(pFileChannel);

        Poco::Logger::root().setChannel(pSplitterChannel);

        Poco::Logger::root().information(Poco::DateTimeFormatter::format(
            Poco::LocalDateTime(),Poco::DateTimeFormat::HTTP_FORMAT));

        Log("Initializing Polygony Engine...");

        if (!result)
        {
            throw Exception("Console allocation failed.",GetLastError());
        }

        Log("Loading configurable options...");

        Poco::AutoPtr<Poco::Util::IniFileConfiguration> pConfigurationFile(
            new Poco::Util::IniFileConfiguration(INI_FILE));

        bool fullScreen = pConfigurationFile->getBool("System.Fullscreen",
            false);
        uint fullscreenWidth = pConfigurationFile->getInt("System.FullscreenX",
            1280);
        uint fullscreenHeight = pConfigurationFile->getInt("System.FullscreenY",
            720);
        uint windowWidth = pConfigurationFile->getInt("System.WindowedX",800);
        uint windowHeight = pConfigurationFile->getInt("System.WindowedY",600);

        uint width = fullScreen ? fullscreenWidth : windowWidth;
        uint height = fullScreen ? fullscreenHeight : windowHeight;

        if (!fullScreen)
        {
            Log("Screen Mode: Windowed " + TO_STRING(width) + "x" +
                TO_STRING(height));
        }
        else
        {
            Log("Screen Mode: Fullscreen " + TO_STRING(width) + "x" +
                TO_STRING(height));
        }

        WNDCLASSEX windowClass;

        //Initialize the window class structure.
        ZeroMemory(&windowClass,sizeof(WNDCLASSEX));

        //Fill in the structure with the needed information.
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.hCursor = LoadCursor(NULL,IDC_ARROW);
        windowClass.hInstance = mInstanceHandle;
        windowClass.lpfnWndProc = WndProc;
        windowClass.lpszClassName = mWindowClass;
        windowClass.style = CS_HREDRAW | CS_VREDRAW;

        //Register the window class.
        RegisterClassEx(&windowClass);

        //Initialize a rectangle with the size of the desired client area.
        RECT windowRect = {0,0,width,height};

        //Adjust the size for the final window.
        AdjustWindowRect(&windowRect,WS_OVERLAPPEDWINDOW,FALSE);

        //Create the window and use the result as the handle.
        mWindowHandle = CreateWindowEx(NULL,mWindowClass,L"Polygony Engine",
            WS_OVERLAPPEDWINDOW,(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
            (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL,NULL,mInstanceHandle,NULL);

        if (mWindowHandle == NULL)
        {
            throw Exception("Window creation failed.",GetLastError());
        }

        //Display the window on the screen.
        ShowWindow(mWindowHandle,SW_SHOW);

        //Bring the window up on the screen and set it as main focus.
        SetForegroundWindow(mWindowHandle);
        SetFocus(mWindowHandle);

        mpInput = new Input(this);
        mpRenderer = new DX11Renderer(this);

        result = mpRenderer->Initialize(width,height,fullScreen,
            pConfigurationFile->getBool("System.VerticalSync",false));

        Log("Initialization took " + TO_STRING(
            static_cast<float>(mStopWatch.elapsed()) /
            static_cast<float>(mStopWatch.resolution())) + " seconds.");

        return result;
    }
    catch (Exception& exception)
    {
        Log(exception.message() + " Error code: " +
            TO_STRING(exception.code()));
    }
    catch (std::exception& exception)
    {
        Log("Error: " + TO_STRING(exception.what()));
    }

    return false;
}

void System::Shutdown()
{
    if (mpInput != NULL)
    {
        delete mpInput;
        mpInput = NULL;
    }

    if (mpRenderer != NULL)
    {
        delete mpRenderer;
        mpRenderer = NULL;
    }

    if (mWindowHandle != NULL)
    {
        DestroyWindow(mWindowHandle);
        mWindowHandle = NULL;
    }

    if (mInstanceHandle != NULL)
    {
        UnregisterClass(mWindowClass,mInstanceHandle);
        mInstanceHandle = NULL;
    }

    if (spSystem != NULL)
    {
        Log("Shutting down Polygony Engine...");
        Poco::Logger::root().information("");

        mStopWatch.stop();

        spSystem = NULL;
    }
}

void System::Log(const string& message)
{
    Poco::Logger::root().information(Poco::cat(Poco::DateTimeFormatter::format(
        Poco::LocalDateTime(),"[%M:%S.%i] "),message));
}

int System::Run(HINSTANCE instanceHandle,const string& commandLine)
{
    mInstanceHandle = instanceHandle;

    bool result = Initialize();

    if (result)
    {
        MSG message;

        //Initialize the message structure.
        ZeroMemory(&message,sizeof(MSG));

        bool loop = true;

        //Loop until there is a quit message from the window or the user.
        while (loop)
        {
            //Handle the Windows messages.
            if (PeekMessage(&message,NULL,0,0,PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }

            //If Windows signals to end the application then exit out.
            if (message.message == WM_QUIT)
            {
                loop = false;
            }
            else
            {
                //Otherwise do the frame processing.
                result = mpRenderer->Render();

                if (!result)
                {
                    loop = false;
                }
            }

        }

        Shutdown();
    }

    if (!result)
    {
        return EXIT_FAILURE; //Means that the application wasn't successful.
    }
    else
    {
        return EXIT_SUCCESS; //Means that the application was successful.
    }
}

bool System::WindowEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
    LPARAM secondParam)
{
    switch (intMessage)
    {
        //Check if the window is being closed or destroyed.
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);

            return true;

        //Check if a key has been pressed on the keyboard.
        case WM_KEYDOWN:
            if (mpInput != NULL)
            {
                return mpInput->KeyPressEvent(
                    static_cast<uint>(firstParam));
            }

        //Check if a key has been released on the keyboard.
        case WM_KEYUP:
            if (mpInput != NULL)
            {
                return mpInput->KeyReleaseEvent(
                    static_cast<uint>(firstParam));
            }

        //Check if the window size has changed.
        case WM_SIZE:
            if (mpRenderer != NULL)
            {
                return mpRenderer->WindowResize();
            }
    }

    return false;
}

HWND System::GetWindowHandle()
{
    return mWindowHandle;
}


}
