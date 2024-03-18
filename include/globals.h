#ifndef LOBSTER_GLOBALS_H
#define LOBSTER_GLOBALS_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <map>
#include <string>

// DECLARES GLOBAL VALUES //
namespace globals
{
	// GLOBAL VARIABLES //
	extern std :: map<std :: string, std :: map<std :: string, pybind11 :: function>> scripts;

	// GLOBAL UTILITY FUNCTIONS //
	void globalInit();
}

#endif
