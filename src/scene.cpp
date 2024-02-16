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

	// READS COMPONENTS FROM CAMERA ENTITY //
	Camera * camera = (Camera *) 
		(targetScene -> components[CAMERA_COMP_ID][cameraEntity]);
	Transform * transform = (Transform *)
		(targetScene -> components[TRANS_COMP_ID][cameraEntity]);
	graphicManagement :: usePipeline(camera -> curPipelineRef);

	// GETS MODEL-WORLD AND WORLD-VIEW MATRICES //
	LobMatrix worldView = cameraHandler :: getWorldViewMatrix(camera, transform);
	LobMatrix viewProj = cameraHandler :: getViewProjMatrix(camera);

	// PUSHES MATRICES TO SHADER //
	glUniformMatrix4fv(UNI_WORLD_VIEW_MATRIX, 1, GL_FALSE, worldView.getData());
	glUniformMatrix4fv(UNI_VIEW_PROJ_MATRIX, 1, GL_FALSE, viewProj.getData());

	// BEGINS ITERATING THROUGH AND RENDERING VALID ENTITIES //
	std :: vector<entityID> meshes = sceneManagement :: sceneView
		(targetScene, MESH_COMP_ID);
	
	for(unsigned i = 0; i < meshes.size(); i++)
	{
		// READS ENTITY INDEX //
		unsigned entityIndex = meshes[i];

		// READS ENTITY DATA //
		Entity curEntity = targetScene -> entities[entityIndex];

		// IF ENTITY HAS TRANSFORM COMPONENT, CONFIGURES MODEL WORLD MATRIX //
			// BY ITS DATA. OTHERWISE, USES MODEL WORLD MATRIX AS IDENTITY MATRIX //
		LobMatrix modelWorld = math :: identityMatrix();
		if((curEntity.mask & (1 << TRANS_COMP_ID)) > 0)
		{
			modelWorld = transformHandler :: getModelWorldMatrix
			(
				(Transform *) 
					(targetScene -> components[TRANS_COMP_ID][entityIndex])
			);
		}

		// SETS MODEL-WORLD MATRIX //
		glUniformMatrix4fv
			(UNI_MODEL_WORLD_MATRIX, 1, GL_FALSE, modelWorld.getData());

		// DRAWS MESH //
		meshHandler :: drawMesh
			((Mesh *) targetScene -> components[MESH_COMP_ID][entityIndex]);
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

		if((curEnt.mask & (1 << compID)) > 0)
			newVec.push_back(entityIndex);

		activeLeft--;
		entityIndex++;
	}

	return newVec;
}

