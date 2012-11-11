
#ifndef POLY_GLOBALS_H
#define POLY_GLOBALS_H


#include <iostream> //Provides input and output functionality using streams. A
                    //stream is an abstraction that represents a device on which
                    //input and ouput operations are performed.
#include <stdlib.h> //Includes several general purpose functions for dynamic
                    //memory management, random number generation,
                    //communication with the environment, integer arithmetics,
                    //searching, sorting and converting.
#include <string> //Includes the string class of the Standard Template Library
                  //(STL).

#include "Boost/lexical_cast.hpp"
#include "Boost/utility.hpp"

#include "Poco/Logger.h"
#include "Poco/Types.h"
#include "Poco/String.h"
#include "Poco/StopWatch.h"

#include "vectormath/vmInclude.h" //An optional header file from Bullet Physics,
                                  //provides functions for 3D/4D vectors,
                                  //3x3/4x4 matrices and quaternions.


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

typedef Vectormath::Aos::Matrix3 Matrix3;
typedef Vectormath::Aos::Matrix4 Matrix4;
typedef Vectormath::Aos::Quat Quat;
typedef Vectormath::Aos::Vector3 Vector3;
typedef Vectormath::Aos::Vector4 Vector4;


#define VMATH Vectormath::Aos

#define INI_FILE "config.ini"
#define PI 3.14159265358979323846f

#define LOG(x) Poly::System::Log(x)
#define TO_STRING(x) boost::lexical_cast<string>(x)

#define DEBUG_INFO TO_STRING(" File: " + TO_STRING(__FILE__).substr((TO_STRING(__FILE__).find_last_of("/\\")) + 1) + "; Line: " + TO_STRING(__LINE__) + ".")


#endif //POLY_GLOBALS_H
