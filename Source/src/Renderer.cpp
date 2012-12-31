
#include "Renderer.h"


#include "System.h"


namespace Poly
{


Renderer::Renderer() :
    mFps(0.0f),
    mFpsIterations(0),
    mFpsLastTimestamp(0.0f),
    mFpsLogging(false)
{
}

Renderer::~Renderer()
{
}

/*virtual*/
void Renderer::Render()
{
    mFpsIterations++;

    float elapsedTime = Poly::System::Get()->ElapsedTime();
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
