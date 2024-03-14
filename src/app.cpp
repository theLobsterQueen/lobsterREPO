// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// (TEMP) FILE-GLOBAL VARIABLES //
static entityID camID = 0; 
static entityID testID = 0;

// FUNCTION IMPLEMENTATIONS //
void appManagement :: begin(EngineCore * core)
{
	// CREATES BASE RENDER PIPELINE //
	Pipeline * basePipeline = graphicManagement :: createPipeline();
	graphicManagement :: loadShader
	(
		basePipeline, GL_VERTEX_SHADER, 
		"shaders/vertShader.txt"
	);

	graphicManagement :: loadShader
	(
		basePipeline, GL_FRAGMENT_SHADER, 
		"shaders/fragShader.txt"
	);

	graphicManagement :: compileProgram(basePipeline);
	core -> pipelineRefs.push_back(basePipeline);

	// CREATES INPUT STATE //
	core -> inputState = inputManagement :: createInputState();

	// CREATES EDITOR DATA STRUCT //
	core -> editorDataRef = editorManagement :: createEditorData(core);

	// (TEMP) CREATES TEST SCENE //
	createTestScene(core);

	// BEGINS RUNNING THE ENGINE //
	run(core);
}

void appManagement :: createTestScene(EngineCore * core)
{
	// CREATES CAMERA COMPONENT //
	Scene * testScene = sceneManagement :: createScene("TEST");
	camID = sceneManagement :: newEntityID(testScene, "Camera");
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
	entityID lightID = sceneManagement :: newEntityID(testScene, "Light");
	Transform * lightTrans = transformHandler :: createTransform();
	sceneManagement :: addComp
	(
		testScene,
		lightID,
		TRANS_COMP_ID,
		(compPtr) lightTrans
	);
	transformHandler :: translate
		(lightTrans, std :: vector<float> { -5.0f, 0.0f, 3.0f });

	sceneManagement :: addComp
	(
		testScene,
		lightID,
		LIGHT_COMP_ID,
		(compPtr) lightHandler :: createLight
			(std :: vector<float> { 1.0f, 1.0f, 1.0f, 0.9f })
	);

	testID = sceneManagement :: newEntityID(testScene, "Jinx");
	Mesh * sceneMesh = meshHandler :: getMeshFromPLY("portrait.ply");
	meshHandler :: setTexture(sceneMesh, textureHandler :: createTexture("jinx.png"));

	// ADDS MESH AND TRANSFORM //
	sceneManagement :: addComp
	(
		testScene, testID, MESH_COMP_ID, 
		(compPtr) (sceneMesh)
	);

	Transform * meshTrans = transformHandler :: createTransform();
	transformHandler :: translate
		(meshTrans, std :: vector<float> { 0.0f, 0.0f, 3.0f });

	sceneManagement :: addComp
	(
		testScene, testID, TRANS_COMP_ID, 
		(compPtr) (meshTrans)
	);

	sceneManagement :: changeScene(core, testScene);
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

		// ATTEMPTS TO FIND CAMERA IN SCENE //
		bool hasCamera = sceneManagement :: getCameraEntityID(core -> curSceneRef, &camID);
		if(!hasCamera)
			for(char i = 0; i < 4; i++)
				core -> clearColor[i] = 0.0f;

		// BEGINS RENDERING PHASE //
		graphicManagement :: beginRenderPass(core);

		// UI WORK //
		// ImGui :: ShowDemoWindow();
		uiManagement :: drawEditorUI(core);

		// RENDERS CURRENT 3D SCENE //
		if(hasCamera)
			sceneManagement :: renderScene(core -> curSceneRef, camID);
		
		// PRESENTS TO SCREEN //
		graphicManagement :: present(core);
	}
}

void appManagement :: update(EngineCore * core)
{
	// VARIABLE INITIALIZATION //
	SDL_Event event;

	// UPDATES MOUSE POSITION VALUE //
	SDL_GetMouseState(&(core -> inputState -> mouseX), &(core -> inputState -> mouseY));

	// IF POLLING RETURNS 0, THERE IS NO PENDING EVENT: RETURNS //
	while(SDL_PollEvent(&event))
	{
		// PROCESSES INPUT FOR IMGUI //
		ImGui_ImplSDL2_ProcessEvent(&event);

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

				if(event.key.keysym.sym == '1')
				{
					sceneManagement :: changeScene(core, sceneManagement :: loadScene("TEST"));
					break;
				}

				if(event.key.keysym.sym == '2')
				{
					sceneManagement :: changeScene(core, sceneManagement :: loadScene("TEST 2"));
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
				core -> inputState -> mouseDeltaX = event.motion.xrel;
				core -> inputState -> mouseDeltaY = event.motion.yrel;
			break;

			case SDL_QUIT :
				core -> isRunning = false;
			break;
		};
	}

	// PROCESSED INPUT //
	if(sceneManagement :: getCameraEntityID(core -> curSceneRef, &camID))
		inputManagement :: processInput(core, camID);

	// RESETS RELATIVE INPUT VALUES //
	inputManagement :: resetInput(core -> inputState);
}
