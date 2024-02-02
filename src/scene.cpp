// INCLUDES DEFINITIONS //
#include <scene.h>

Scene * sceneManagement :: createScene(std :: string inputName)
{
	Scene * newScene = new Scene;
	newScene -> name = inputName;
	return newScene;
}

void sceneManagement :: changeScene(EngineCore * core, Scene * targetScene)
{
	core -> curSceneRef = targetScene;
	windowManagement :: changeTitle(core -> winRef, targetScene -> name);
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
	if(targetScene -> entities[entityIndex].mask == 0)
		return;

	targetScene -> entities[entityIndex].mask = 0;
	targetScene -> activeEntities--;
}

void sceneManagement :: addComp
	(Scene * targetScene, entityID entityIndex, componentID compIndex, compPtr inputCompPtr)
{ 
	// IF ENTITY MASK IS EMPTY BEFORE ADDING, INCREMENTS ACTIVE ENTITY COUNTER //
	if(targetScene -> entities[entityIndex].mask == 0)
		targetScene -> activeEntities++;

	// ADDS POINTER TO COMPONENT CONTAINER //
	targetScene -> components[compIndex][entityIndex] = inputCompPtr; 

	// MODIFIES ENTITY BITMASK //
	targetScene -> entities[entityIndex].mask |= (1 << compIndex);
}

void sceneManagement :: renderScene(EngineCore * core)
{
	// VARIABLE INITIALIZATION //
	Scene * targetScene = core -> curSceneRef;

	// BEGINS ITERATING THROUGH AND RENDERING VALID ENTITIES //
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

		// READS ENTITY AND SAVES MESH ID //
		Entity temp = targetScene -> entities[entityIndex];
		componentID meshID = componentManagement :: getID<Mesh>();

		// THEN, DRAWS MESH IF ONE IS EQUIPPED ON THE ENTITY //
		if((temp.mask & (1 << meshID)) > 0)
			meshManagement :: draw
				(core, (Mesh *) (targetScene -> components[meshID][entityIndex]));

		// REDUCES ACTIVE ENTITY COUNT AND INCREASING ENTITY INDEX //
			// FOR NEXT ITERATION //
		entityCount--;
		entityIndex++;
	}
}
