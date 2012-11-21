
#include "System.h"


#include "Poco/DateTimeFormatter.h"


namespace Poly
{


static System* spSystem = nullptr;


System::System() :
    mErrorCode(0),

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
void System::Log(const string& message)
{
    Poco::Logger::root().information(Poco::cat(Poco::DateTimeFormatter::format(
        Poco::LocalDateTime(),"[%M:%S.%i] "),message));
}

/*virtual*/
void System::Run(const string& commandLine)
{
    Initialize();

    if (NoErrorCode())
    {
        //Game loop should go here.
    }

    Shutdown();
}

/*static*/
System* System::Get()
{
    return spSystem;
};


}
