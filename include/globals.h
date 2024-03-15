#ifndef LOBSTER_GLOBALS_H
#define LOBSTER_GLOBALS_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <map>

// LOBSTER INCLUDES //
#include <script.h>

// STRUCT PROTOTYPES //
struct Script;

// DECLARES GLOBAL VALUES //
namespace globals
{
	extern std :: map<std :: string, Script *> scripts;
}

#endif
