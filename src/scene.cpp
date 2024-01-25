// INCLUDES DEFINITIONS //
#include <scene.h>

Scene sceneManagement :: createScene(std :: string inputName)
{
	Scene newScene;
	newScene.name = inputName;
	return newScene;
}

entityID sceneManagement :: newEntityID(Scene * targetScene)
{
	entityID entityIndex = 0;
	while(targetScene -> entities[entityIndex].mask != 0)
		entityIndex++;

	return entityIndex;
}

void sceneManagement :: deleteEntity(Scene * targetScene, entityID entityIndex)
{
	targetScene -> entities[entityIndex].mask = 0;
}

void sceneManagement :: addComp
	(Scene * targetScene, entityID entityIndex, componentID compIndex, char * compPtr)
{ 
	// IF ENTITY MASK IS EMPTY BEFORE ADDING, INCREMENTS ACTIVE ENTITY COUNTER //
	if(targetScene -> entities[entityIndex].mask == 0)
		targetScene -> activeEntities++;

	// ADDS POINTER TO COMPONENT CONTAINER //
	targetScene -> components[entityIndex][compIndex] = compPtr; 

	// MODIFIES ENTITY BITMASK //
	targetScene -> entities[entityIndex].mask |= (1 << compIndex);
}

void sceneManagement :: renderScene(Scene * targetScene, Pipeline * inputPipeline)
{
	unsigned entityCount = targetScene -> activeEntities;
	unsigned entityIndex = 0;
	while(entityCount != 0 && entityIndex < LOBSTER_MAX_ENTITIES)
	{
		// IF ENTITY'S COMPONENT MASK IS 0 (EMPTY/DELETED), CONTINUES //
		if(targetScene -> entities[entityIndex].mask == 0)
		{
			entityIndex++;
			continue;
		}

		// MASK IS NOT 0, REDUCES TEMP ENTITY COUNT BY 1 //
		entityCount--;

		// READS ENTITY AND SAVES MESH ID //
		Entity temp = targetScene -> entities[entityIndex];
		componentID meshID = componentManagement :: getID<Mesh>();

		// THEN, DRAWS MESH IF ONE IS EQUIPPED ON THE ENTITY //
		if((temp.mask & (1 << meshID)) == 1)
			mesh :: draw
			(
				(Mesh *) (targetScene -> components[meshID][entityIndex]), 
				inputPipeline
			);
	}
}
