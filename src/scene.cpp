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
// PARAMETERS //
(
	Scene * targetScene, 
	entityID entityIndex, 
	componentID compIndex, 
	compPtr inputCompPtr
)

// FUNCTION //
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

		// READS ENTITY DATA //
		Entity curEntity = targetScene -> entities[entityIndex];

		// THEN, DRAWS MESH IF ONE IS EQUIPPED ON THE ENTITY //
		if((curEntity.mask & (1 << MESH_COMP_ID)) > 0)
		{
			// IF ENTITY HAS TRANSFORM COMPONENT, GRABS IT //
				// OTHERWISE, SETS WORLD MATRIX TO IDENTITY MATRIX //
			LobMatrix worldMat = math :: identityMatrix();
			if((curEntity.mask & (1 << TRANS_COMP_ID)) > 0)
				worldMat = transformHandler :: getWorldMat
					((Transform *) targetScene -> components[TRANS_COMP_ID][entityIndex]);

			// SETS OBJECT WORLD MATRIX //
			glUniformMatrix4fv
			(
				UNI_WORLD_MATRIX,  // WHAT UNIFORM IT'S SETTING //
				1, GL_FALSE, worldMat.getData()
			);

			// DRAWS MESH //
			meshHandler :: drawMesh
				((Mesh *) targetScene -> components[MESH_COMP_ID][entityIndex]);
		}

		// REDUCES ACTIVE ENTITY COUNT AND INCREASING ENTITY INDEX //
			// FOR NEXT ITERATION //
		entityCount--;
		entityIndex++;
	}
}
