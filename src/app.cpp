// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// (TEMP) FILE-GLOBAL VARIABLES //
static entityID camID = 0;
static entityID testID = 0;
static Scene * testScene = nullptr;

// FUNCTION IMPLEMENTATIONS //
void appManagement :: begin(EngineCore * core)
{
	// INITIALIZES CORE VALUES //
	testScene = sceneManagement :: createScene("Lobter Home");
	sceneManagement :: changeScene(core, testScene);

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

	// CREATES INPUT STATE //
	core -> inputState = inputManagement :: createInputState();

	// (TEMP) CREATES TEST SCENE //
	createTestScene(core);

	// BEGINS RUNNING THE ENGINE //
	run(core);
}

void appManagement :: createTestScene(EngineCore * core)
{
	// CREATES CAMERA COMPONENT //
	camID = sceneManagement :: newEntityID(testScene);
	float aspect = ((float) core -> winWidth) / ((float) core -> winHeight);

	sceneManagement :: addComp
	(
		testScene,
		camID,
		CAMERA_COMP_ID,
		(compPtr) cameraHandler :: createCamera
			(aspect, 70, core -> pipelineRefs[0])
	);

	sceneManagement :: addComp
	(
		testScene,
		camID,
		TRANS_COMP_ID,
		(compPtr) transformHandler :: createTransform()
	);

	// ADDS LIGHT TO SCENE //
	entityID lightID = sceneManagement :: newEntityID(testScene);
	Transform * lightTrans = transformHandler :: createTransform();
	sceneManagement :: addComp
	(
		testScene,
		lightID,
		TRANS_COMP_ID,
		(compPtr) lightTrans
	);
	transformHandler :: translate
		(lightTrans, std :: vector<float> { -5.0f, 0.0f, 0.0f });

	sceneManagement :: addComp
	(
		testScene,
		lightID,
		LIGHT_COMP_ID,
		(compPtr) lightHandler :: createLight
			(std :: vector<float> { 1.0f, 0.5f, 0.5f, 0.75f })
	);

	// CREATES SCENE //
	testID = sceneManagement :: newEntityID(testScene);
	Mesh * sceneMesh = meshHandler :: getMeshFromPLY("portrait.ply");
	meshHandler :: setTexture(sceneMesh, textureHandler :: createTexture("jinx.png"));

	// ADDS MESH AND TRANSFORM //
	sceneManagement :: addComp
	(
		testScene, testID, MESH_COMP_ID, 
		(compPtr) (sceneMesh)
	);
	sceneManagement :: addComp
	(
		testScene, testID, TRANS_COMP_ID, 
		(compPtr) (transformHandler :: createTransform())
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
				// SETS MODIFIER KEYS //
				modifierKeyProcess(core -> inputState, event, true);

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
				// SETS MODIFIER KEYS //
				modifierKeyProcess(core -> inputState, event, false);

				// IF ABOVE NORMAL ASCII RANGE, IGNORES //
				if(event.key.keysym.sym > 128)
					break;

				// REMOVES THE KEY FROM THE PRESSED KEYS BITMASK //
				core -> inputState -> pressedKeys[event.key.keysym.sym] = 0;
			break;

			case SDL_MOUSEWHEEL :
				core -> inputState -> scroll = event.wheel.y;
			break;

			case SDL_MOUSEBUTTONDOWN :
				mouseButtonProcess(core -> inputState, event, true);	
			break;
	
			case SDL_MOUSEBUTTONUP :
				mouseButtonProcess(core -> inputState, event, false);	
			break;

			case SDL_MOUSEMOTION :
				core -> inputState -> mouseX = event.motion.xrel;
				core -> inputState -> mouseY = event.motion.yrel;
			break;

			case SDL_QUIT :
				core -> isRunning = false;
			break;
		};
	}

	// PROCESSED INPUT //
	inputManagement :: processInput(core, camID);

	// RESETS RELATIVE INPUT VALUES //
	inputManagement :: resetInput(core -> inputState);
}
