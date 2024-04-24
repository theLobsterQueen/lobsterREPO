// INCLUDES DECLARATION FILE //
#include <entity.h>

// FUNCTION DEFINITIONS //
void entityHandler :: processOrder
	(std :: string orderName, entityID entID, std :: vector<pybind11 :: object>& params)
{
	// ITERATES THROUGH ALL ACTIVE ENTITIES IN THE SCENE, AND RETURNS THE ID //
		// OF THE ONE WHICH CORRESPONDS WITH THE INPUT NAME //
	if(orderName == "getEntityByName")
	{
		std :: string entName = pybind11 :: cast<std :: string>(params[0]);
		unsigned short actives = globals :: curSceneRef -> activeEntities;
		for(unsigned i = 0; i < LOBSTER_MAX_ENTITIES && actives > 0; i++)
		{
			if(globals :: curSceneRef -> entities[i].mask == 0)
				continue;
			actives--;
			std :: string tempName = globals :: curSceneRef -> entities[i].name;
			if(!(strcmp(tempName.c_str(), entName.c_str())))
			{
				params[1].attr("id") = i;
				params[1].attr("ready") = true;
				return;
			}
		}

		std :: cout << "COULD NOT FIND ENTITY OF NAME " << entName << "!" << std :: endl;
		params[1].attr("id") = LOBSTER_MAX_ENTITIES + 1;
	}

	if(orderName == "addEntity")
	{
		std :: string entName = pybind11 :: cast<std :: string>(params[0]);
		entityID newEnt = sceneManagement :: newEntityID(globals :: curSceneRef, entName);
		params[1].attr("id") = newEnt;
		APIGlobals :: coremodule.attr("name_ids")[entName.c_str()] = newEnt;
	}

	if(orderName == "deleteEntity")
		{ sceneManagement :: deleteEntity(globals :: curSceneRef, entID); }
}
