
#include "System.h"


#include "Poco/DateTimeFormatter.h"


namespace Poly
{


static System* spSystem = nullptr;


System::System() : Runnable(),
    mCleanedUp(false),
    mErrorCode(0),

    mDesiredFramerate(60.0f),
    mDynamicSleepSecs(0.0f),
    mOldElapsedTime(0.0f),

    mpInput(nullptr),
    mpRenderer(nullptr)
{
    spSystem = this;
}

System::~System()
{
    if (mpRenderer.get() != nullptr)
    {
        mpRenderer.reset(nullptr);
    }

    if (mpInput != nullptr)
    {
        mpInput.reset(nullptr);
    }

    Poco::Logger::root().information("");

    spSystem = nullptr;
}

/*static*/
System* System::Get()
{
    return spSystem;
};

/*static*/
void System::Log(const string& message)
{
    Poco::Logger::root().information(Poco::cat(Poco::DateTimeFormatter::format(
        Poco::LocalDateTime(),"[%M:%S.%i] "),message));
}

/*virtual*/
void System::Run()
{
    Initialize();

    if (NoErrorCode())
    {
        //Game loop should go here.
    }

    Shutdown();
}

/*virtual*/
void System::ThreadDynamicSleep()
{
    float elapsedTime = ElapsedTime();

    if (boost::algorithm::clamp(elapsedTime - mOldElapsedTime,0.0001f,1.0f) <
        1.0f / mDesiredFramerate)
    {
        mDynamicSleepSecs += 0.001f;
    }
    else
    {
        mDynamicSleepSecs = std::max(0.0f,mDynamicSleepSecs - 0.001f);
    }

    mOldElapsedTime = elapsedTime;

    if (mDynamicSleepSecs > 0.0f)
    {
        ThreadSleep(mDynamicSleepSecs);
    }
}

void System::ThreadSleep(float seconds)
{
    Poco::Thread::sleep(static_cast<long>(seconds * 1000.0f));
}


}
