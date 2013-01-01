
#include "Renderer.h"


#include "Application.h"


namespace Poly
{


Renderer::Renderer() :
    mFps(0.0f),
    mFpsIterations(0),
    mFpsLastTimestamp(0.0f),
    mFpsLogging(false),

    mVerticalSync(false)
{
}

Renderer::~Renderer()
{
}

/*virtual*/
void Renderer::Initialize(uint width,uint height,bool fullScreen)
{
    mFpsLogging = Poly::Application::Get()->ConfigurationFile()->getBool(
        "Renderer.LogFPS",false);
    mVerticalSync = Poly::Application::Get()->ConfigurationFile()->getBool(
        "Renderer.VerticalSync",false);
}

/*virtual*/
void Renderer::Render()
{
    mFpsIterations++;

    float elapsedTime = Poly::Application::Get()->ElapsedTime();
    float timeDifference = boost::algorithm::clamp(elapsedTime -
        mFpsLastTimestamp,0.0001f,1.0f);

    if (timeDifference >= 0.1f)
    {
        mFps = mFpsIterations / static_cast<float>(timeDifference);

        mFpsIterations = 0;
        mFpsLastTimestamp = elapsedTime;

        if (mFpsLogging)
        {
            LOG(TO_STRING(mFps) + " FPS");
        }
    }
}


}
