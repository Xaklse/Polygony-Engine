
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
    Poco::Util::AbstractConfiguration* pConfigurationFile =
        Poly::Application::Get()->ConfigurationFile();

    mFpsLogging = pConfigurationFile->getBool("Renderer.LogFPS",false);
    mShadersPath = pConfigurationFile->getString("Renderer.ShadersPath","");
    mVerticalSync = pConfigurationFile->getBool("Renderer.VerticalSync",false);
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

/*virtual*/
void Renderer::CalculateOrthographicMatrix(float width,float height,
    float nearPlane,float farPlane)
{
    //Create an orthographic projection matrix for 2D rendering; uses a
    //left-handed coordinate system.
    //    /  2/w   0      0     0  \
    //    |   0   2/h     0     0  |
    //    |   0    0   1/(f-n)  0  |
    //    \   0    0   n/(n-f)  1  /
    mOrthographicMatrix << 2.0f / width,                      0.0f,0.0f,0.0f,
                           0.0f,2.0f / height,                     0.0f,0.0f,
                           0.0f,0.0f,1.0f / (farPlane - nearPlane),     0.0f,
                           0.0f,0.0f,nearPlane / (nearPlane - farPlane),1.0f;
}

/*virtual*/
void Renderer::CalculatePerspectiveMatrix(float width,float height,
    float nearPlane,float farPlane,float fovY)
{
    //Create the perspective projection matrix based on a field of view for 3D
    //rendering; uses a left-handed coordinate system.
    //    /  cot(f/2)*h/w     0            0        0  \
    //    |        0       cot(f/2)        0        0  |
    //    |        0          0       zf/(zf-zn)    1  |
    //    \        0          0     -zn*zf/(zf-zn)  0  /
    float cotangent = tan(PI * 0.5f - fovY * 0.5f);
    float range = 1.0f / (farPlane - nearPlane);

    mPerspectiveMatrix << cotangent * height / width,   0.0f,0.0f,0.0f,
                          0.0f,cotangent,                    0.0f,0.0f,
                          0.0f,0.0f,farPlane * range,             1.0f,
                          0.0f,0.0f,-nearPlane * farPlane * range,0.0f;
}


}
