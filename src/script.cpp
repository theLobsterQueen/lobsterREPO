// INCLUDES DECLARATION FILE //
#include <script.h>

// FUNCTION DEFINITIONS //
Script * scriptHandler :: createScript(std :: string scriptName, entityID entRef)
{
	// CREATES SCRIPT REFERENCE AND RETURNS //
	Script * newScript = new Script;
	newScript -> name = scriptName;
	newScript -> code = pybind11 :: globals()[newScript -> name.c_str()]();
	return newScript;
}
