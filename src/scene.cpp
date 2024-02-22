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

entityID sceneManagement :: newEntityID(Scene * targetScene, std :: string entityName)
{
	entityID entityIndex = 0;
	while(targetScene -> entities[entityIndex].mask != 0)
		entityIndex++;

	targetScene -> entities[entityIndex].name = entityName;
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

	// GETS ALL LIGHT SOURCES AND PUSHES THEIR POSITIONS TO THE SHADERS //
		// TODO: ADD SUPPORT FOR MULTIPLE LIGHT SOURCES AND DIFFERENT LIGHT COLORS //
	entityID lightEnt = sceneManagement :: sceneView(targetScene, LIGHT_COMP_ID)[0];
	Light light = *((Light *) targetScene ->
		components[LIGHT_COMP_ID][lightEnt]);
	glUniform4f
	(
		UNI_LIGHT_COLOR,
		light.color[0], light.color[1], light.color[2], light.color[3]
	);
	Transform lightTrans = *((Transform *) targetScene -> 
		components[TRANS_COMP_ID][lightEnt]);
	glUniform3f
	(
		UNI_LIGHT_POS, 
		lightTrans.position[0], lightTrans.position[0], lightTrans.position[0]
	);

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

void sceneManagement :: saveScene(Scene * inputScene)
{
	// ATTEMPTS TO OPEN THE FILE FROM WORKING DIRECTORY //
	std :: ofstream sceneFile
		(std :: string("./scenes/" + inputScene -> name).c_str());

	// PUTS THE NAME OF THE SCENE AT THE TOP OF THE FILE // 
	sceneFile << inputScene -> name << std :: endl;

	// IF SCENE FILE DIRECTORY WAS FOUND SUCCESSFULLY, BEGINS WRITING FILE. //
	unsigned short int activeCount = inputScene -> activeEntities;
	unsigned i = 0;
	while(activeCount > 0)
	{
		// IF ENTITY WITH AN INACTIVE MASK, SKIPS OVER AND CONTINUES //
		if(inputScene -> entities[i].mask == 0)
		{
			i++;
			continue;
		}

		// IF ENTITY WITH AN ACTIVE MASK, DETERMINES WHICH COMPONENTS IT USES //
			// AND SAVES THEIR DATA //
		// WRITES OUT ENTITY NAME //
		Entity curEnt = inputScene -> entities[i];
		sceneFile << "ENT: " << curEnt.name << std :: endl;

		// CHECKS FOR TRANSFORMS //
		if((curEnt.mask & (1 << TRANS_COMP_ID)) > 0)
		{
			Transform * tempTrans = (Transform *) 
				inputScene -> components[TRANS_COMP_ID][i];
			sceneFile << "TRANSFORM" << std :: endl
				<< tempTrans -> position[0] << "/" << tempTrans -> position[1]
				<< "/" << tempTrans -> position[2] << std :: endl
				<< tempTrans -> rotation[0] << "/" << tempTrans -> rotation[1]
				<< "/" << tempTrans -> rotation[2] << std :: endl
				<< tempTrans -> scale[0] << "/" << tempTrans -> scale[1]
				<< "/" << tempTrans -> scale[2] << std :: endl;
		}

		// CHECKS FOR LIGHTS //
		if((curEnt.mask & (1 << LIGHT_COMP_ID)) > 0)
		{
			Light * tempLight = (Light *)
				inputScene -> components[LIGHT_COMP_ID][i];
			sceneFile << "LIGHT" << std :: endl
				<< tempLight -> color[0] << "/" << tempLight -> color[1]
				<< "/" << tempLight -> color[2] << "/" << tempLight -> color[3]
				<< std :: endl;
		}

		// CHECKS FOR CAMERAS //
		if((curEnt.mask & (1 << CAMERA_COMP_ID)) > 0)
		{
			Camera * tempCamera = (Camera *)
				inputScene -> components[CAMERA_COMP_ID][i];
			sceneFile << "CAMERA" << std :: endl
				<< tempCamera -> FOV << std :: endl
				<< tempCamera -> near << std :: endl
				<< tempCamera -> far << std :: endl;
		}

		// SAVES MESHES AND TEXTURES //
		if((curEnt.mask & (1 << MESH_COMP_ID)) > 0)
		{
			Mesh * tempMesh = (Mesh *)
				inputScene -> components[MESH_COMP_ID][i];
			sceneFile << "MESH" << std :: endl
				<< tempMesh -> name << std :: endl
				<< tempMesh -> texName << std :: endl;
		}

		// INCREMENTS INDEX AND REDUCES ACTIVE COUNT //
		i++;
		activeCount--;
	}
}

Scene * sceneManagement :: loadScene(std :: string scenePath)
{
	// VARIABLE INITIALIZATION //
	unsigned linesRead = 0;
	int compIndex = -1;

	Scene * newScene;
	
	// FINDS AND READS SCENE FILE //
	std :: string sceneString, line;
	std :: ifstream sceneFile(std :: string("scenes/") + scenePath);
	if(!sceneFile.is_open())
	{
		// ATTEMPTS TO LOAD IT FROM THE PERSEPCTIVE OF AN EXECUTABLE IN DEBUG/RELEASE //
		sceneFile.open(std :: string("../../scenes") + scenePath);
		if(!sceneFile.is_open())
		{
			std :: cout << "COULD NOT LOAD SCENE AT " << scenePath << "!" << std :: endl;
			return nullptr;
		}
	}

	// GETS SCENE NAME //
	std :: cout << "LOADED SCENE FILE!" << std :: endl;
	std :: getline(sceneFile, line);
	newScene -> name = line;

	// READS SCENE DATA //
	entityID newEntity = 0;
	compPtr newCompPtr = nullptr;
	std :: string meshName = ""; std :: string texName = "";

	while(std :: getline(sceneFile, line))
	{
		// READS ENTITY NAMES //
		if(line.find(std :: string("Ent")) != std :: string :: npos)
		{
			compIndex = -1;
			linesRead = -2;

			std :: stringstream lineStream(line);
			std :: string data;

			int index = 0;
			while(std :: getline(lineStream, data, ' '))
			{
				if(index == 1)
					newEntity = sceneManagement :: newEntityID(newScene, data);
				index++;
			}
		}

		// READS COMPONENT DATA //
		std :: stringstream lineStream(line);
		std :: string data;
		if(compIndex == TRANS_COMP_ID)
		{
			Transform * tempTrans = ((Transform *) (newCompPtr));
			if(linesRead == 0)
			{
				// SETS POSITION //
				int index = 0;
				while(std :: getline(lineStream, data, '/'))
					tempTrans -> position[index] = std :: stof(data.c_str());
			}

			// SETS ROTATION //
			else if(linesRead == 1) 
			{
				int index = 0;
				while(std :: getline(lineStream, data, '/'))
					tempTrans -> rotation[index] = std :: stof(data.c_str());
			}

			// SETS ROTATION //
			else if(linesRead == 2)
			{
				int index = 0;
				while(std :: getline(lineStream, data, '/'))
					tempTrans -> scale[index] = std :: stoi(data.c_str());
			}
		}
		
		else if(compIndex == LIGHT_COMP_ID)
		{
			Light * lightPtr = ((Light *) (newCompPtr));
			int index = 0;
			if(linesRead == 0)
				while(std :: getline(lineStream, data, '/'))
					lightPtr -> color[index] = std :: stoi(data.c_str());
		}

		else if(compIndex == MESH_COMP_ID)
		{
			if(linesRead == 0)
				meshName = line;

			else if(linesRead == 1)
				texName = line;
		}

		else if(compIndex == CAMERA_COMP_ID)
		{
			Camera * cameraPtr = ((Camera *) (newCompPtr));
			if(linesRead == 0)
				cameraPtr -> FOV = std :: stof(line.c_str());
			if(linesRead == 1)
				cameraPtr -> near = std :: stoi(line.c_str());
			if(linesRead == 2)
				cameraPtr -> far = std :: stoi(line.c_str());
		}

		// CHECKS FOR TRANSFORMS //
		if(line.find(std :: string("TRANSFORM")) != std :: string :: npos)
		{
			// ADDS TRANSFORM COMPONENT //
			if(compIndex != -1)
			{
				compPtr tempPtr = (compPtr) newCompPtr;
				if(compIndex == MESH_COMP_ID)
				{
					tempPtr = (compPtr) 
						(meshHandler :: getMeshFromPLY(meshName));
					meshHandler :: setTexture
						((Mesh *) tempPtr, textureHandler :: createTexture(texName));
				}

				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, (compPtr) tempPtr
				);
			}

			newCompPtr = (compPtr) transformHandler :: createTransform();
			compIndex = TRANS_COMP_ID;
			linesRead = 0;
		}

		// CHECKS FOR LIGHTS //
		if(line.find(std :: string("LIGHT")) != std :: string :: npos)
		{
			// ADDS TRANSFORM COMPONENT //
			if(compIndex != -1)
			{
				compPtr tempPtr = (compPtr) newCompPtr;
				if(compIndex == MESH_COMP_ID)
				{
					tempPtr = (compPtr) 
						(meshHandler :: getMeshFromPLY(meshName));
					meshHandler :: setTexture
						((Mesh *) tempPtr, textureHandler :: createTexture(texName));
				}

				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, (compPtr) tempPtr
				);
			}

			newCompPtr = (compPtr) lightHandler :: createLight();
			unsigned char linesRead = 0;
			compIndex = LIGHT_COMP_ID;
		}

		// CHECKS FOR CAMERAS //
		if(line.find(std :: string("CAMERA")) != std :: string :: npos)
		{
			if(compIndex != -1)
			{
				compPtr tempPtr = (compPtr) newCompPtr;
				if(compIndex == MESH_COMP_ID)
				{
					tempPtr = (compPtr) 
						(meshHandler :: getMeshFromPLY(meshName));
					meshHandler :: setTexture
						((Mesh *) tempPtr, textureHandler :: createTexture(texName));
				}

				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, (compPtr) tempPtr
				);
			}

			newCompPtr = (compPtr) cameraHandler :: createCamera();
			unsigned char linesRead = 0;
			compIndex = CAMERA_COMP_ID;
		}

		// SAVES MESHES AND TEXTURES //
		if(line.find(std :: string("MESH")) != std :: string :: npos)
		{
			if(compIndex != -1)
			{
				compPtr tempPtr = (compPtr) newCompPtr;
				if(compIndex == MESH_COMP_ID)
				{
					tempPtr = (compPtr) 
						(meshHandler :: getMeshFromPLY(meshName));
					meshHandler :: setTexture
						((Mesh *) tempPtr, textureHandler :: createTexture(texName));
				}

				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, (compPtr) tempPtr
				);
			}

			unsigned char linesRead = 0;
			compIndex = MESH_COMP_ID;
		}

		linesRead++;
	}

	return newScene;
}
