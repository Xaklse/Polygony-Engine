
#ifndef POLY_APPLICATION_H
#define POLY_APPLICATION_H


#include "Globals.h"
#include "Renderer.h"
#include "Input.h"


namespace Poly
{


class Application : public Poco::Runnable, private boost::noncopyable
{
public:
    Application();
    virtual ~Application();

    void run() { Run(); }

    virtual void Exit() = 0;
    virtual void Run();

    static Application* Get();

    Poco::Util::AbstractConfiguration* ConfigurationFile()
        { return mpConfigurationFile.get(); }
    float ElapsedTime() const
        { return static_cast<float>(mStopWatch.elapsed()) /
          static_cast<float>(mStopWatch.resolution()); }

    void CommandLine(const string& commandLine) { mCommandLine = commandLine; }
    void ThreadSleep(float seconds);

    int ErrorCode() const { return mErrorCode; }
    void ErrorCode(int errorCode) { mErrorCode = errorCode; }
    bool NoErrorCode() const { return mErrorCode == 0; }

protected:
    virtual void CleanUp() = 0;
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void ThreadDynamicSleep();

    bool mCleanedUp;
    string mCommandLine;
    int mErrorCode;

    float mDesiredFramerate;
    float mDynamicSleepSecs;
    float mOldElapsedTime;

    std::unique_ptr<Input> mpInput;
    std::unique_ptr<Renderer> mpRenderer;

    Poco::AutoPtr<Poco::Util::AbstractConfiguration> mpConfigurationFile;
    Poco::Stopwatch mStopWatch;
};


}


#endif //POLY_APPLICATION_H
