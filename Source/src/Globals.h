
#ifndef POLY_GLOBALS_H
#define POLY_GLOBALS_H


//Turns off certain problematic macros related to header files of Windows.
#define NOMINMAX
#define VC_EXTRALEAN
#define WINDOWS_LEAN_AND_MEAN

//Enables macros mainly used for debugging and/or development.
#ifdef _DEBUG
    #define POLY_DEBUG_MEMORY
    #define POLY_DEBUG_SHADERS
    #define POLY_SHOW_CONSOLE
#endif


#include <algorithm> //Defines a collection of functions especially designed to
                     //be used on ranges of elements.
#include <array> //Includes the static array container of the Standard Template
                 //Library (STL).
#include <iostream> //Provides input and output functionality using streams. A
                    //stream is an abstraction that represents a device on which
                    //input and output operations are performed.
#include <memory> //Provides general utilities to manage memory.
#include <stdlib.h> //Includes several general purpose functions for dynamic
                    //memory management, random number generation,
                    //communication with the environment, integer arithmetics,
                    //searching, sorting and converting.
#include <string> //Includes the string class of the Standard Template Library
                  //(STL).

#ifdef POLY_DEBUG_MEMORY
    #include <crtdbg.h> //Keeps track of memory allocation and deallocation.
#endif

#include "Boost/algorithm/clamp.hpp"
#include "Boost/lexical_cast.hpp"
#include "Boost/math/constants/constants.hpp"
#include "Boost/utility.hpp"

#include "Poco/AutoPtr.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Environment.h"
#include "Poco/Logger.h"
#include "Poco/Runnable.h"
#include "Poco/StopWatch.h"
#include "Poco/String.h"
#include "Poco/Thread.h"
#include "Poco/Types.h"
#include "Poco/Util/AbstractConfiguration.h"

#include "Eigen/Core"
#include "Eigen/Geometry"


typedef unsigned int uint;

typedef Poco::Int8 int8;
typedef Poco::Int16 int16;
typedef Poco::Int32 int32;
typedef Poco::Int64 int64;

typedef Poco::UInt8 uint8;
typedef Poco::UInt16 uint16;
typedef Poco::UInt32 uint32;
typedef Poco::UInt64 uint64;

typedef float float32;
typedef double float64;

typedef std::string string;

typedef Eigen::Matrix<float,3,3,Eigen::RowMajor> Matrix3;
typedef Eigen::Matrix<float,3,3,Eigen::RowMajor | Eigen::DontAlign> Matrix3DA;
typedef Eigen::Matrix<float,4,4,Eigen::RowMajor> Matrix4;
typedef Eigen::Matrix<float,4,4,Eigen::RowMajor | Eigen::DontAlign> Matrix4DA;
typedef Eigen::Quaternion<float> Quat;
typedef Eigen::Matrix<float,3,1> Vector3;
typedef Eigen::Matrix<float,3,1,Eigen::DontAlign> Vector3DA;
typedef Eigen::Matrix<float,4,1> Vector4;
typedef Eigen::Matrix<float,4,1,Eigen::DontAlign> Vector4DA;


const string CONF_FILE = "config.ini";
const string LOG_FILE = "log.log";

const float PI = boost::math::float_constants::pi;


#ifdef POLY_DEBUG_MEMORY
    #define NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#else
    #define NEW new
#endif


#define LOG(x) Poly::Log(x)
#define TO_STRING(x) boost::lexical_cast<string>(x)

#define DEBUG_INFO TO_STRING(" File: " + TO_STRING(__FILE__).substr((TO_STRING(__FILE__).find_last_of("/\\")) + 1) + "; Line: " + TO_STRING(__LINE__) + ".")


namespace Poly
{


static void Log(const string& message)
{
    Poco::Logger::root().information(Poco::cat(Poco::DateTimeFormatter::format(
        Poco::LocalDateTime(),"[%M:%S.%i] "),message));
}


}


#endif //POLY_GLOBALS_H
