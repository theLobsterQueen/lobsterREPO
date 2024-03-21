#ifndef LOBSTER_API_UTILS
#define LOBSTER_API_UTILS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <string>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <scene.h>

// HOLDS ALL OPAQUE DECLARATIONS //
PYBIND11_MAKE_OPAQUE(Scene);
PYBIND11_MAKE_OPAQUE(Transform);
PYBIND11_MAKE_OPAQUE(Light);

// DECLARES NAMESPACE-GLOBAL VARIABLES //
namespace APIGlobals
{
	extern pybind11 :: module coremodule;
	extern std :: string workingPath;
}

#endif
