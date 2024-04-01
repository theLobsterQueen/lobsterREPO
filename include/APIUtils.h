#ifndef LOBSTER_API_UTILS
#define LOBSTER_API_UTILS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <string>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <scene.h>
#include <input.h>
#include <mesh.h>

// DECLARES NAMESPACE-GLOBAL VARIABLES //
namespace APIGlobals
{
	extern pybind11 :: module coremodule;
	extern pybind11 :: module inputmodule;
	extern pybind11 :: module appmodule;
	extern std :: string workingPath;
}

#endif
