// INCLUDES DECLARATION FILE //
#include <script.h>

// FUNCTION DEFINITIONS //
Script * scriptHandler :: createScript(std :: string scriptName, entityID entRef)
{
	// CREATES SCRIPT REFERENCE AND RETURNS //
	Script * newScript = new Script;
	if(scriptName != "")
	{
		if(APIGlobals :: coremodule.attr("script_refs").contains(scriptName.c_str()))
		{ 
			newScript -> name = scriptName;
			newScript -> id = entRef;
			newScript -> code = APIGlobals :: coremodule.attr("script_refs")[scriptName.c_str()]();
		}

		else
		{
			std :: cout << "ERROR! COULDN'T FIND " << scriptName << std :: endl;
			return nullptr;
		}
	}

	return newScript;
}
