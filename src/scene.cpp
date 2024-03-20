// INCLUDES DEFINITIONS //
#include <scene.h>

Scene * sceneManagement :: createScene(std :: string inputName)
{
	Scene * newScene = new Scene;
	newScene -> name = inputName;
	return newScene;
}

void sceneManagement :: changeScene(Scene * targetScene)
{
	// ASSIGNS SCENE TO ATTRIBUTE IN COREMODULE //
	APIGlobals :: coremodule.attr("scene_ref") = (*targetScene);

	entityID camID;
	if(sceneManagement :: getCameraEntityID(targetScene, &camID))
	{
		globals :: curSceneRef = targetScene;
		Camera * temp = (Camera *) targetScene -> components[CAMERA_COMP_ID][camID];
		temp -> aspectRatio = (((float) (globals :: winWidth)) / ((float) (globals :: winHeight)));
		windowManagement :: changeTitle(globals :: winRef, targetScene -> name);
	}

	else
	{
		std :: cout << "ATTEMPTED TO SWITCH TO " << targetScene -> name << " BUT COULD NOT FIND"
			<< " A SUITABLE CAMERA TO RENDER TO!" << std :: endl;
		return;
	}
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

void sceneManagement :: renderScene(Scene * targetScene)
{
	entityID cameraEntity;
	if(!sceneManagement :: getCameraEntityID(targetScene, &cameraEntity))
	{
		std :: cout << "ERROR! ATTEMPTED TO RENDER A SCENE THAT DOES NOT HAVE A CAMERA!"
			<< std :: endl;
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
		lightTrans.position[0], lightTrans.position[1], lightTrans.position[2]
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
	std :: vector<entityID> newVec = { }; unsigned entityIndex = 0;
	for(Entity curEnt : inputScene -> entities)
	{
		if((curEnt.mask & (1 << compID)) > 0)
			newVec.push_back(entityIndex);
		entityIndex++;
	}

	return newVec;
}

void sceneManagement :: saveScene(Scene * inputScene)
{
	// ATTEMPTS TO OPEN THE FILE FROM WORKING DIRECTORY //
	std :: ofstream sceneFile
		(std :: string("./scenes/" + inputScene -> name + ".lscn").c_str());
	if(!sceneFile.is_open())
		sceneFile = std :: ofstream
			(std :: string("./../../scenes/" + inputScene -> name + ".lscn").c_str());

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
				<< tempCamera -> near << std :: endl
				<< tempCamera -> far << std :: endl
				<< tempCamera -> curPipelineRef -> vertShaderName << std :: endl
				<< tempCamera -> curPipelineRef -> fragShaderName << std :: endl;
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

	// PUTS IN "END" LINE TO DENOTE END OF SAVE FILE //
	sceneFile << "END" << std :: endl;
}

Scene * sceneManagement :: loadScene(std :: string scenePath)
{
	// VARIABLE INITIALIZATION //
	int linesRead = 0;
	int compIndex = -1;
	
	// FINDS AND READS SCENE FILE //
	std :: string sceneString, line;
	std :: cout << "LOADING SCENE: " << scenePath << std :: endl;
	std :: ifstream sceneFile("./scenes/" + scenePath);
	if(!sceneFile.is_open())
	{
		// ATTEMPTS TO LOAD IT FROM THE PERSEPCTIVE OF AN EXECUTABLE IN DEBUG/RELEASE //
		sceneFile.open("./../../scenes/" + scenePath);
		if(!sceneFile.is_open())
		{
			std :: cout << "COULD NOT LOAD SCENE AT " << scenePath << "!" << std :: endl;
			return nullptr;
		}
	}

	// GETS SCENE NAME //
	std :: getline(sceneFile, line);
	Scene * newScene = sceneManagement :: createScene(line);

	// READS SCENE DATA //
	entityID newEntity = 0;
	compPtr newCompPtr = nullptr;

	while(std :: getline(sceneFile, line))
	{
		// READS ENTITY NAMES //
		if(line.find(std :: string("ENT")) != std :: string :: npos)
		{
			if(compIndex != -1)
			{
				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, newCompPtr
				);
			}

			compIndex = -1;
			std :: stringstream lineStream(line);
			std :: string data;
			unsigned char index = 0;
			while(lineStream >> data)
			{
				if(index == 1)
					newEntity = sceneManagement :: newEntityID(newScene, data);
				index++;
			}
		}

		if(line.find(std :: string("END")) != std :: string :: npos)
		{
			if(compIndex != -1)
			{
				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, newCompPtr
				);
			}
		}

		// READS COMPONENT DATA //
		std :: stringstream lineStream(line);
		std :: string data;
		if(compIndex == TRANS_COMP_ID)
		{
			int index = 0;
			Transform * tempTrans = ((Transform *) (newCompPtr));
			if(linesRead == 0)
			{
				// SETS POSITION //
				while(std :: getline(lineStream, data, '/'))
				{
					tempTrans -> position[index] = std :: stof(data.c_str());
					index++;
				}
			}

			// SETS ROTATION //
			else if(linesRead == 1) 
			{
				while(std :: getline(lineStream, data, '/'))
				{
					tempTrans -> rotation[index] = std :: stof(data.c_str());
					index++;
				}
			}

			// SETS SCALE //
			else if(linesRead == 2)
			{
				while(std :: getline(lineStream, data, '/'))
				{
					tempTrans -> scale[index] = std :: stof(data.c_str());
					index++;
				}
			}
		}
		
		if(compIndex == LIGHT_COMP_ID)
		{
			Light * lightPtr = ((Light *) (newCompPtr));
			int index = 0;
			if(linesRead == 0)
				while(std :: getline(lineStream, data, '/'))
				{
					lightPtr -> color[index] = std :: stof(data.c_str());
					index++;
				}
		}

		if(compIndex == MESH_COMP_ID)
		{
			if(linesRead == 0)
				newCompPtr = ((compPtr) (meshHandler :: getMeshFromPLY(line)));

			else if(linesRead == 1)
			{
				Mesh * meshPtr = ((Mesh *) (newCompPtr));
				meshHandler :: setTexture(meshPtr, textureHandler :: createTexture(line));
			}
		}

		if(compIndex == CAMERA_COMP_ID)
		{
			Camera * cameraPtr = ((Camera *) (newCompPtr));
			if(linesRead == 0)
				cameraPtr -> near = std :: stof(line.c_str());
			if(linesRead == 1)
				cameraPtr -> far = std :: stoi(line.c_str());
			if(linesRead == 2)
			{
				graphicManagement :: loadShader
					(cameraPtr -> curPipelineRef, GL_VERTEX_SHADER, line.c_str());
			}

			if(linesRead == 3)
			{
				graphicManagement :: loadShader
					(cameraPtr -> curPipelineRef, GL_FRAGMENT_SHADER, line.c_str());
			}
		}

		// CHECKS FOR LOADING DIFFERENTS COMPONENTS //

		// CHECKS FOR TRANSFORMS //
		if(line.find(std :: string("TRANSFORM")) != std :: string :: npos)
		{
			// ADDS PREVIOUSLY CONFIGURED COMPONENT, IF ANY //
			if(compIndex != -1)
			{
				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, newCompPtr
				);
			}

			newCompPtr = (compPtr) transformHandler :: createTransform();
			compIndex = TRANS_COMP_ID;
			linesRead = -1;
		}

		// CHECKS FOR LIGHTS //
		if(line.find(std :: string("LIGHT")) != std :: string :: npos)
		{
			// ADDS A PREVIOUSLY CONFIGURED COMPONENT, IF ANY //
			if(compIndex != -1)
			{
				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, newCompPtr
				);
			}

			newCompPtr = (compPtr) lightHandler :: createLight();
			compIndex = LIGHT_COMP_ID;
			linesRead = -1;
		}

		// CHECKS FOR CAMERAS //
		if(line.find(std :: string("CAMERA")) != std :: string :: npos)
		{
			// ADDS PREVIOUSLY CONFIGURED COMPONENT, IF ANY //
			if(compIndex != -1)
			{
				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, newCompPtr
				);
			}

			newCompPtr = (compPtr) cameraHandler :: createCamera();
			compIndex = CAMERA_COMP_ID;
			linesRead = -1;
		}

		// CHECKS FOR MESHES //
		if(line.find(std :: string("MESH")) != std :: string :: npos)
		{
			// ADDS PREVIOUSLY CONFIGURED COMPONENT, IF ANY //
			if(compIndex != -1)
			{
				sceneManagement :: addComp
				(
					newScene, newEntity,
					compIndex, newCompPtr
				);
			}

			newCompPtr = nullptr;
			compIndex = MESH_COMP_ID;
			linesRead = -1;
		}

		linesRead++;
	}

	return newScene;
}

bool sceneManagement :: getCameraEntityID(Scene * targetScene, entityID * entHolder)
{
	for(entityID i = 0; i < LOBSTER_MAX_ENTITIES; i++)
	{
		// IF FOUND AN ENTITY HOLDING A CAMERA, GRABS ITS ID AND RETURNS TRUE //
		if(targetScene -> components[CAMERA_COMP_ID][i] != 0)
		{
			(*entHolder) = i;
			return true;
		}
	}

	// IF ONE COULD NOT BE FOUND, RETURNS FALSE //
	return false;
}

void sceneManagement :: sceneOut(Scene * inputScene)
{
	for(unsigned i = 0; i < LOBSTER_MAX_ENTITIES; i++)
	{
		Entity curEntity = inputScene -> entities[i];
		if(curEntity.mask == 0)
			continue;

		std :: cout << "ENTITY NAME: " << curEntity.name << std :: endl;
		if((curEntity.mask & (1 << TRANS_COMP_ID)) >= 1)
			std :: cout << "\tHAS TRANSFORM!" << std :: endl;
		if((curEntity.mask & (1 << MESH_COMP_ID)) >= 1)
			std :: cout << "\tHAS MESH!" << std :: endl;
		if((curEntity.mask & (1 << LIGHT_COMP_ID)) >= 1)
			std :: cout << "\tHAS LIGHT!" << std :: endl;
		if((curEntity.mask & (1 << CAMERA_COMP_ID)) >= 1)
			std :: cout << "\tHAS CAMERA!" << std :: endl;
	}
}

void sceneManagement :: updateScene(Scene * inputScene, float deltaTime)
{
	// CALLS THE UPDATE FUNCTION //
	for(entityID curEnt : sceneManagement :: sceneView(inputScene, SCRIPT_COMP_ID))
	{
		Script * script = ((Script *) (inputScene -> components[SCRIPT_COMP_ID][curEnt]));
		script -> code.attr("_update")(deltaTime);

		// GETS DATA FROM SCRIPT AND APPLIES IT //
		pybind11 :: dict retValue = script -> code.attr("_push_data")();
		Entity entRef = inputScene -> entities[curEnt];
		sceneManagement :: setData<std :: string>(retValue, "parent_name", &(entRef.name));
		sceneManagement :: setData<Transform>
		(
		 	retValue, "transform",
			(Transform *) (inputScene -> components[TRANS_COMP_ID][curEnt])
		);
		inputScene -> entities[curEnt] = entRef;
	}
}


