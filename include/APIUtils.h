#ifndef LOBSTER_API_UTILS
#define LOBSTER_API_UTILS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <string>

// DECLARES NAMESPACE-GLOBAL VARIABLES //
namespace APIGlobals
{
	extern pybind11 :: module coremodule;
	extern std :: string workingPath;
}

#endif
