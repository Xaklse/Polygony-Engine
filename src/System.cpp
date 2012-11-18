
#include "System.h"


#include "Poco/DateTimeFormatter.h"


namespace Poly
{


static System* spSystem = nullptr;


System::System()
{
    spSystem = this;

    mErrorCode = 0;

    mpInput = nullptr;
    mpRenderer = nullptr;
}

System::~System()
{
    if (mpRenderer != nullptr)
    {
        delete mpRenderer;
        mpRenderer = nullptr;
    }

    if (mpInput != nullptr)
    {
        delete mpInput;
        mpInput = nullptr;
    }

    Poco::Logger::root().information("");

    spSystem = nullptr;
}

/*virtual*/
void System::Exit()
{
}

/*virtual*/
void System::Initialize()
{
}

/*virtual*/
void System::Shutdown()
{
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
