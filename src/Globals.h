
#ifndef POLY_GLOBALS_H
#define POLY_GLOBALS_H


#include "Poco/Types.h"


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


#define PI 3.14159265358979323846f


#define INI_FILE "config.ini"
#define LOG mpSystem->Log
#define TO_STRING boost::lexical_cast<string>


#endif //POLY_GLOBALS_H
