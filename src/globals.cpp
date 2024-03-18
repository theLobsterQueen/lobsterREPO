// INCLUDES DEFINITION FILE //
#include <globals.h>

// DEFINES GLOBAL VALUES //
std :: map<std :: string, std :: map<std :: string, pybind11 :: function>> globals :: scripts;

// INITIALIZES GLOBAL VALUES //
void globals :: globalInit()
{
	globals :: scripts = std :: map<std :: string, std :: map<std :: string, pybind11 :: function>> { };
}
