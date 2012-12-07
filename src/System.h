
#ifndef POLY_SYSTEM_H
#define POLY_SYSTEM_H


#include "Globals.h"
#include "Renderer.h"
#include "Input.h"


namespace Poly
{


class System : public Poco::Runnable, private boost::noncopyable
{
public:
    System();
    virtual ~System();

    void run() { Run(); }

    virtual void Exit() = 0;
    static void Log(const string& message);
    virtual void Run();

    static System* Get();

    void CommandLine(const string& commandLine) { mCommandLine = commandLine; }
    float ElapsedTime() const
        { return static_cast<float>(mStopWatch.elapsed()) /
          static_cast<float>(mStopWatch.resolution()); }
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

    Poco::Stopwatch mStopWatch;
};


}


#endif //POLY_SYSTEM_H
