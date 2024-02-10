// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// (TEMP) FILE-GLOBAL VARIABLES //
static entityID testID = 0;
static entityID camID = 0;
static bool movingCamera = false;

// FUNCTION IMPLEMENTATIONS //
void appManagement :: begin(EngineCore * core)
{
	// INITIALIZES CORE VALUES //
	core -> curSceneRef = sceneManagement :: createScene();

	// CREATES BASE RENDER PIPELINE //
	Pipeline * basePipeline = graphicManagement :: createPipeline();
	graphicManagement :: loadShader
	(
		basePipeline, GL_VERTEX_SHADER, 
		"shaders/vertShader.txt", core -> debug
	);
	graphicManagement :: loadShader
	(
		basePipeline, GL_FRAGMENT_SHADER, 
		"shaders/fragShader.txt", core -> debug
	);

	graphicManagement :: compileProgram(basePipeline);
	core -> pipelineRefs.push_back(basePipeline);

	// CHANGES THE NAME OF THE WINDOW TO THE NEW CURRENT SCENE //
	windowManagement :: changeTitle(core -> winRef, core -> curSceneRef -> name);

	// (TEMP) CREATES TEST SCENE //
	createTestScene(core);

	// BEGINS RUNNING THE ENGINE //
	run(core);
}

void appManagement :: createTestScene(EngineCore * core)
{
	entityID testID = sceneManagement :: newEntityID(core -> curSceneRef);
	
	// FISHIES!!! //
	sceneManagement :: addComp
	(
		core -> curSceneRef,
		testID, MESH_COMP_ID,
		(compPtr) meshHandler :: getMeshFromPLY("./assets/models/cube.ply")
	);

	sceneManagement :: addComp
	(
		core -> curSceneRef,
		testID, TRANS_COMP_ID,
		(compPtr) transformHandler :: createTransform()
	);

	// OFFSETS FISH //
	transformHandler :: translate
	(
		(Transform *) (core -> curSceneRef -> components[TRANS_COMP_ID][testID]),
		std :: vector<float> { 0.0f, 0.0f, -10.0f }
	);

	transformHandler :: rotate
	(
		(Transform *) (core -> curSceneRef -> components[TRANS_COMP_ID][testID]),
		std :: vector<float> { 90.0f, 0.0f, 0.0f }
	);

	// CREATES CAMERA COMPONENT //
	camID = sceneManagement :: newEntityID(core -> curSceneRef);
	float aspect = ((float) core -> winWidth) / ((float) core -> winHeight);
	std :: cout << "ASPECT RATIO IS: " << aspect << "!" << std :: endl;

	sceneManagement :: addComp
	(
		core -> curSceneRef,
		camID,
		CAMERA_COMP_ID,
		(compPtr) cameraHandler :: createCamera
			(aspect, 70, core -> pipelineRefs[0])
	);

	sceneManagement :: addComp
	(
		core -> curSceneRef,
		camID,
		TRANS_COMP_ID,
		(compPtr) transformHandler :: createTransform()
	);
}

void appManagement :: run(EngineCore * core)
{
	// VARIABLE INITIALIZATION //
	unsigned long int newDelta = SDL_GetTicks();
	unsigned long int oldDelta = 0;
	static float timer = 0.0f;

	while(core -> isRunning)
	{
		// UPDATES DELTA TIME //
		oldDelta = newDelta;
		newDelta = SDL_GetTicks();
		
		core -> deltaTime = (newDelta - oldDelta) / 1000.0f;

		// UPDATES ENGINE //
		appManagement :: update(core);

		// BEGINS RENDERING PHASE //
		graphicManagement :: beginRenderPass(core);

		// TEST FUNCTIONS //
		if(movingCamera)
			testFuncs :: processInput(core, camID);

		else
			testFuncs :: processInput(core, testID);

		// RENDERS CURRENT SCENE //
		sceneManagement :: renderScene(core -> curSceneRef, camID);
		
		// PRESENTS TO SCREEN //
		graphicManagement :: present(core);
	}
}

void appManagement :: update(EngineCore * core)
{
	// VARIABLE INITIALIZATION //
	SDL_Event event;

	// IF POLLING RETURNS 0, THERE IS NO PENDING EVENT: RETURNS //
	while(SDL_PollEvent(&event))
	{
		char justPressed[64] = {0};
		switch(event.type)
		{
			case SDL_KEYDOWN :
				// IF ABOVE NORMAL ASCII RANGE, IGNORES //
				if(event.key.keysym.sym > 128)
					break;

				// IF READING ESCAPE KEY, QUITS //
				if(event.key.keysym.sym == 27)
				{
					core -> isRunning = false;
					break;
				}

				// ADDS THE KEY TO THE PRESSED KEYS BITMASK //
				core -> inputState -> pressedKeys[event.key.keysym.sym] = 1;

				// ADDS THE KEY TO THE NEXT AVAILABLE ENTRY IN "justPressed" //
				if(event.key.repeat == 0)
					for(char i = 0; i < 64; i++)
						if(justPressed[i] == 0)
						{
							justPressed[i] = event.key.keysym.sym;
							break;
						}
			break;

			case SDL_KEYUP :
				// IF ABOVE NORMAL ASCII RANGE, IGNORES //
				if(event.key.keysym.sym > 128)
					break;

				// REMOVES THE KEY FROM THE PRESSED KEYS BITMASK //
				core -> inputState -> pressedKeys[event.key.keysym.sym] = 0;
			break;

			case SDL_QUIT :
				core -> isRunning = false;
			break;
		};

		// (TODO) CALL USER SCRIPTS' "UPDATE" FUNCTIONS //
			// FOR NOW, JUST DOES SOME DEBUG WORK //
		for(char i = 0; i < 16; i++)
		{
			switch(justPressed[i])
			{
				case ' ' :
					movingCamera = !movingCamera;
					std :: cout << "CHANGING TO: ";
					if(movingCamera)
						std :: cout << "CAMERA!" << std :: endl;
					else
						std :: cout << "MESH!" << std :: endl;
				break;
			}
		}
	}
}
