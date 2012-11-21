
#include "WinSystem.h"


#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/FileChannel.h"
#include "Poco/LocalDateTime.h"
#include "Poco/SplitterChannel.h"
#include "Poco/WindowsConsoleChannel.h"


#include "DX11Renderer.h"
#include "Exception.h"


/**
 * The entry point for any program based on Windows.
 * @param instanceHandle  integer which identifies this application
 * @param hPrevInstance   obsolete
 * @param lpCommandLine   long pointer to string that contains the command line
 * @param nShowCommand    indicates how the window is to appear when created
 * @return                exit value of the program; 0 means no errors
 */
int WINAPI WinMain(HINSTANCE instanceHandle,HINSTANCE hPrevInstance,
    LPSTR lpCommandLine,int nShowCommand)
{
    Poly::System* pSystem = new Poly::WinSystem();

    pSystem->Run(static_cast<string>(lpCommandLine));

    int result = pSystem->ErrorCode();

    delete pSystem;

    return result;
}

/**
 * Receives all input directed at a window.
 * @param windowHandle  unique handle of the window
 * @param intMessage    received message
 * @param firstParam    first extra value
 * @param secondParam   second extra value
 * @return              exit value; 0 means the message has been handled
 */
LRESULT CALLBACK WndProc(HWND windowHandle,UINT intMessage,WPARAM firstParam,
    LPARAM secondParam)
{
    if (!static_cast<Poly::WinSystem*>(Poly::System::Get())->WindowEvent(
        windowHandle,intMessage,firstParam,secondParam))
    {
        //Send the event to the default message handler.
        return DefWindowProc(windowHandle,intMessage,firstParam,secondParam);
    }

    return 0;
}


namespace Poly
{


WinSystem::WinSystem() : System(),
    mpConfigurationFile(nullptr),
    mInstanceHandle(nullptr),
    mWindowHandle(nullptr)
{
}

WinSystem::~WinSystem()
{
    if (mWindowHandle != nullptr)
    {
        DestroyWindow(mWindowHandle);
        mWindowHandle = nullptr;
    }

    if (mInstanceHandle != nullptr)
    {
        UnregisterClass(mWindowClassName,mInstanceHandle);
        mInstanceHandle = nullptr;
    }

    if (mpConfigurationFile.get() != nullptr)
    {
        mpConfigurationFile = nullptr;
    }
}

/*virtual*/
void WinSystem::Exit()
{
    DestroyWindow(mWindowHandle);
}

/*virtual*/
void WinSystem::Initialize()
{
    try
    {
        mInstanceHandle = GetModuleHandle(nullptr);
        mWindowClassName = L"PolygonyEngine";

        //Start the time to measure it.
        mStopWatch.start();

        //Allocate a Windows console if it doesn't exist.
        bool result = AllocConsole() != 0;

        //Create a POCO channel assigned to the Windows console.
        Poco::AutoPtr<Poco::WindowsConsoleChannel> pConsoleChannel(
            new Poco::WindowsConsoleChannel());

        //Create a POCO channel assigned to a logging file.
        Poco::AutoPtr<Poco::FileChannel> pFileChannel(new Poco::FileChannel());
        pFileChannel->setProperty("path",LOG_FILE);
        pFileChannel->setProperty("rotation","10 M");

        //Create a POCO channel that sends a message to the console channel and
        //the logging file simultaneously.
        Poco::AutoPtr<Poco::SplitterChannel> pSplitterChannel(
            new Poco::SplitterChannel());
        pSplitterChannel->addChannel(pConsoleChannel);
        pSplitterChannel->addChannel(pFileChannel);

        //Assign the new splitter channel to the logger object.
        Poco::Logger::root().setChannel(pSplitterChannel);

        //Log the current date and time.
        Poco::Logger::root().information(Poco::DateTimeFormatter::format(
            Poco::LocalDateTime(),Poco::DateTimeFormat::HTTP_FORMAT));

        LOG("Initializing Polygony Engine...");

        if (!result)
        {
            throw Exception("Console allocation failed." + DEBUG_INFO,
                GetLastError());
        }

        LOG(Poco::Environment::osName() + " " + Poco::Environment::osVersion());

        LOG("Loading configurable options...");

        //Load the system settings from the main configuration file.
        mpConfigurationFile = new Poco::Util::IniFileConfiguration(INI_FILE);

        bool fullScreen = mpConfigurationFile->getBool("System.Fullscreen",
            false);
        uint fullscreenWidth = mpConfigurationFile->getInt("System.FullscreenX",
            1280);
        uint fullscreenHeight = mpConfigurationFile->getInt(
            "System.FullscreenY",720);
        uint windowWidth = mpConfigurationFile->getInt("System.WindowedX",800);
        uint windowHeight = mpConfigurationFile->getInt("System.WindowedY",600);
        string windowTitle = mpConfigurationFile->getString(
            "System.WindowTitle","Polygony Engine");

        uint width = fullScreen ? fullscreenWidth : windowWidth;
        uint height = fullScreen ? fullscreenHeight : windowHeight;

        if (!fullScreen)
        {
            LOG("Screen Mode: Windowed " + TO_STRING(width) + "x" +
                TO_STRING(height));
        }
        else
        {
            LOG("Screen Mode: Fullscreen " + TO_STRING(width) + "x" +
                TO_STRING(height));
        }

        WNDCLASSEX windowClass;

        //Initialize the window class structure.
        ZeroMemory(&windowClass,sizeof(WNDCLASSEX));

        //Fill in the structure with the needed information.
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.hCursor = LoadCursor(nullptr,IDC_ARROW);
        windowClass.hInstance = mInstanceHandle;
        windowClass.lpfnWndProc = WndProc;
        windowClass.lpszClassName = mWindowClassName;
        windowClass.style = CS_HREDRAW | CS_VREDRAW;

        //Register the window class.
        RegisterClassEx(&windowClass);

        //Initialize a rectangle with the size of the desired client area.
        RECT windowRect = {0,0,width,height};

        //Adjust the size for the final window.
        AdjustWindowRect(&windowRect,WS_OVERLAPPEDWINDOW,FALSE);

        //Create the window and use the result as the handle.
        mWindowHandle = CreateWindowEx(0,mWindowClassName,
            (std::wstring(windowTitle.begin(),windowTitle.end())).c_str(),
            WS_OVERLAPPEDWINDOW,(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
            (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,nullptr,mInstanceHandle,nullptr);

        if (mWindowHandle == nullptr)
        {
            throw Exception("Window creation failed." + DEBUG_INFO,
                GetLastError());
        }

        //Display the window on the screen.
        ShowWindow(mWindowHandle,SW_SHOW);

        //Bring the window up on the screen and set it as main focus.
        SetForegroundWindow(mWindowHandle);
        SetFocus(mWindowHandle);

        mpInput.reset(new Input());
        mpRenderer.reset(new DX11Renderer());

        mpRenderer->Initialize(width,height,fullScreen);

        LOG("Initialization took " + TO_STRING(
            static_cast<float>(mStopWatch.elapsed()) /
            static_cast<float>(mStopWatch.resolution())) + " seconds.");
    }
    catch (Exception& exception)
    {
        LOG(exception.message() + " Error code: " +
            TO_STRING(exception.code()));

        MessageBox(nullptr,L"Error; check the log file.",mWindowClassName,
            MB_OK | MB_ICONERROR);

        ErrorCode(exception.code() != 0 ? exception.code() : -1);
    }
    catch (std::exception& exception)
    {
        LOG("Error: " + TO_STRING(exception.what()));

        ErrorCode(-1);
    }
}

/*virtual*/
void WinSystem::Shutdown()
{
    LOG("Shutting down Polygony Engine...");

    if (mpRenderer.get() != nullptr)
    {
        mpRenderer.reset(nullptr);
    }

    if (mpInput != nullptr)
    {
        mpInput.reset(nullptr);
    }

    if (mWindowHandle != nullptr)
    {
        DestroyWindow(mWindowHandle);
        mWindowHandle = nullptr;
    }

    if (mInstanceHandle != nullptr)
    {
        UnregisterClass(mWindowClassName,mInstanceHandle);
        mInstanceHandle = nullptr;
    }

    if (mpConfigurationFile.get() != nullptr)
    {
        mpConfigurationFile = nullptr;
    }

    //Stop the measured time.
    mStopWatch.stop();
}

/*virtual*/
void WinSystem::Run(const string& commandLine)
{
    Initialize();

    if (NoErrorCode())
    {
        MSG message;

        //Initialize the message structure.
        ZeroMemory(&message,sizeof(MSG));

        bool loop = true;

        //Loop until there is a quit message from the window or the user.
        while (loop)
        {
            //Handle the Windows messages.
            if (PeekMessage(&message,nullptr,0,0,PM_REMOVE))
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
                mpRenderer->Render();

                loop = NoErrorCode();
            }
        }
    }

    Shutdown();
}

/*virtual*/
bool WinSystem::WindowEvent(HWND windowHandle,UINT intMessage,WPARAM firstParam,
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
            if (mpInput != nullptr)
            {
                return mpInput->KeyPressEvent(static_cast<uint>(firstParam));
            }

        //Check if a key has been released on the keyboard.
        case WM_KEYUP:
            if (mpInput != nullptr)
            {
                return mpInput->KeyReleaseEvent(static_cast<uint>(firstParam));
            }

        //Check if the window size has changed.
        case WM_SIZE:
            if (mpRenderer != nullptr)
            {
                return mpRenderer->WindowResize();
            }
    }

    return false;
}


}
