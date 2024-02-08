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

void sceneManagement :: renderScene(Scene * targetScene, entityID cameraEntity)
{
	// ESURES THAT THE INPUT ENTITY DOES IN FACT HAVE A CAMERA //
	if
	(
		targetScene -> entities[cameraEntity].mask & (1 << CAMERA_COMP_ID) == 0 || 
			targetScene -> entities[cameraEntity].mask & (1 << TRANS_COMP_ID) == 0
	)

	{
		std :: cout << "ERROR! ATTEMPTED TO RENDER SCENE BY INPUTTING AN ENTITY "
			<< "NOT EQUIPPED WITH A CAMERA OR TRANSFORM!" << std :: endl;
		return;
	}

	// USES THE CAMERA'S SELECTED PIPELINE //
	Camera * camera = (Camera *) 
		(targetScene -> components[CAMERA_COMP_ID][cameraEntity]);
	graphicManagement :: usePipeline(camera -> curPipelineRef);

	// PUSHES CAMERA MATRIX TO SHADER //
	glUniformMatrix4fv
	(
		UNI_CAMERA_MATRIX,  // WHAT UNIFORM IT'S SETTING //
		1, GL_FALSE, cameraHandler :: 
		getViewMatrix
		  (camera, (Transform *) (targetScene -> components[TRANS_COMP_ID][cameraEntity])
		).getData()
	);

	// CREATES PROJECTION MATRIX DATA //
	float projMatrix[16] =
	{
		// FIRST COLUMN //
		camera -> hozFOV, 0, 0, 0,

		// SECOND COLUMN //
		0, camera -> verFOV, 0, 0,

		// THIRD COLUMN //
		0, 0, camera -> far / (camera -> near - camera -> far), -1,

		// FOURTH COLUMN //
		0, 0, (camera -> far * camera -> near) / (camera -> near - camera -> far), 0
	};

	// ACTIVES CURRENT PIPELINE AND PUSHES PROJECTION MATRIX //
	glUniformMatrix4fv(UNI_PROJ_MATRIX, 1, GL_FALSE, projMatrix);

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

std :: vector<entityID> sceneManagement :: sceneView
	(Scene * inputScene, componentID compID)
{
	std :: vector<entityID> newVec;
	unsigned int activeLeft = inputScene -> activeEntities;
	unsigned short int entityIndex = 0;
	Entity curEnt;
	while(activeLeft > 0 && entityIndex < LOBSTER_MAX_ENTITIES)
	{
		curEnt = inputScene -> entities[entityIndex];

		if(curEnt.mask == 0)
		{
			entityIndex++;
			continue;
		}

		if(curEnt.mask & (1 << compID) > 0)
			newVec.push_back(entityIndex);

		activeLeft--;
	}

	return newVec;
}

