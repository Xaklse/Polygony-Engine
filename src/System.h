
#ifndef POLY_SYSTEM_H
#define POLY_SYSTEM_H


#include "Globals.h"
#include "Renderer.h"
#include "Input.h"


namespace Poly
{


class System : private boost::noncopyable
{
public:
    System();
    virtual ~System();

    virtual void Exit();
    static void Log(const string& message);
    virtual void Run(const string& commandLine);

    static System* Get();

    int ErrorCode() const { return mErrorCode; }
    void ErrorCode(int errorCode) { mErrorCode = errorCode; }
    bool NoErrorCode() const { return mErrorCode == 0; }

protected:
    virtual void Initialize();
    virtual void Shutdown();

    int mErrorCode;

    Input* mpInput;
    Renderer* mpRenderer;

    Poco::Stopwatch mStopWatch;
};


}


#endif //POLY_SYSTEM_H
