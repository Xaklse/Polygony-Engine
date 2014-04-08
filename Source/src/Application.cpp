
#include "Application.h"


namespace Poly
{


static Application* spApplication = nullptr;


Application::Application() : Runnable(),
    mCleanedUp(false),
    mErrorCode(0),

    mDesiredFramerate(60.0f),
    mDynamicSleepSecs(0.0f),
    mOldElapsedTime(0.0f),

    mpInput(nullptr),
    mpRenderer(nullptr),

    mpConfigurationFile(nullptr)
{
    spApplication = this;
}

Application::~Application()
{
    if (mpConfigurationFile.get() != nullptr)
    {
        mpConfigurationFile = nullptr;
    }

    if (mpRenderer.get() != nullptr)
    {
        mpRenderer.reset(nullptr);
    }

    if (mpInput != nullptr)
    {
        mpInput.reset(nullptr);
    }

    Poco::Logger::root().information("");

    spApplication = nullptr;
}

/*static*/
Application* Application::Get()
{
    return spApplication;
};

/*virtual*/
void Application::Run()
{
    Initialize();

    if (NoErrorCode())
    {
        //Game loop should go here.
    }

    Shutdown();
}

/*virtual*/
void Application::ThreadDynamicSleep()
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

void Application::ThreadSleep(float seconds)
{
    Poco::Thread::sleep(static_cast<long>(seconds * 1000.0f));
}


}
