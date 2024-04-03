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

void scriptHandler :: processOrder
	(std :: string orderName, entityID entID, std :: vector<pybind11 :: object> params)
{
	if(orderName == "addScript")
	{
		sceneManagement :: addComp
		(
			globals :: curSceneRef, entID,
			SCRIPT_COMP_ID, constructComp(SCRIPT_COMP_ID)
		);
	}

	if(orderName == "setScript")
	{
		sceneManagement :: addComp
		(
			globals :: curSceneRef, entID,
			SCRIPT_COMP_ID, (compPtr) (scriptHandler :: createScript
				(pybind11 :: cast<std :: string>(params[0]), entID))
		);
		Script * script = ((Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID][entID])); 
		script -> code.attr("_awake")();
		sceneManagement :: pullOrders(globals :: curSceneRef);
		script -> code.attr("_start")();
	}
}
