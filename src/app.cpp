// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// FUNCTION IMPLEMENTATIONS //
void appManagement :: begin()
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
	globals :: pipelineRefs.push_back(basePipeline);

	// CREATES INPUT STATE //
	globals :: inputState = inputManagement :: createInputState();

	// PREPARES PYTHON CONTEXT AND COMPILES THE SCRIPTS //
	appManagement :: initializeAPI();
	appManagement :: compileScripts();

	// (TEMP) CREATES TEST SCENE //
	appManagement :: createTestScene();

	// BEGINS RUNNING THE ENGINE //
	appManagement :: run();
}

void appManagement :: createTestScene()
{
	// CREATES CAMERA COMPONENT //
	Scene * testScene = sceneManagement :: createScene("TEST");
	entityID camID = sceneManagement :: newEntityID(testScene, "Camera");
	float aspect = ((float) globals :: winWidth) / ((float) globals :: winHeight);

	sceneManagement :: addComp
	(
		testScene,
		camID,
		CAMERA_COMP_ID,
		(compPtr) cameraHandler :: createCamera
			(aspect, globals :: pipelineRefs[0])
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

	entityID testID = sceneManagement :: newEntityID(testScene, "Jinx");
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

	// GIVES MESH A TEST SCRIPT //
	sceneManagement :: addComp
	(
		testScene, testID, SCRIPT_COMP_ID,
		(compPtr) (scriptHandler :: createScript("testFile", testID))
	);
	sceneManagement :: changeScene(testScene);
	Scene * loadedScene = sceneManagement :: loadScene("TEST.lscn");
	sceneManagement :: changeScene(loadedScene);
	sceneManagement :: changeScene(testScene);
}

void appManagement :: initializeAPI()
{
	// VARIABLE INITIALIZATION //
	APIGlobals :: workingPath = "./";
	std :: filesystem :: directory_iterator testDir;
	try
	{
		testDir = std :: filesystem :: begin(std :: filesystem :: directory_iterator
				(std :: string(APIGlobals :: workingPath + "scripts/").c_str())); 
	}

	catch(std :: filesystem :: filesystem_error err)
		{ APIGlobals :: workingPath = "./../../"; }

	// INITIALIZES THE PYTHON INTERPRETER //
	pybind11 :: initialize_interpreter();

	// PREPARES THE PYTHON CONTEXT //
	pybind11 :: exec(std :: string
		("import sys\nsys.path.append(\"" + APIGlobals :: workingPath + "modulesAPI/\")").c_str());
	APIGlobals :: coremodule = pybind11 :: module_ :: import("coremodule");
}

void appManagement :: compileScripts()
{
	// VARIABLE INITIALIZATION //
	static bool firstTimeCompile = true;

	// FINDS SCRIPT DIRECTORY //
	std :: filesystem :: directory_iterator dirIt = 
		std :: filesystem :: begin(std :: filesystem :: directory_iterator(std :: string
		(APIGlobals :: workingPath + "scripts/").c_str())); 

	// RUNS ALL SCRIPTS IN THE SCRIPT DIRECTORY //
	for(std :: filesystem :: directory_entry fileEntry : dirIt)
	{
		// DETERMINES WHETHER THE CURRENT FILE IS A PYTHON FILE //
		std :: string fileName = fileEntry.path().string();
		if(fileName.find(".py") == std :: string :: npos)
			continue;

		// RECONFIGURES THE FILE NAME TO REFER TO THE FILE ITSELF, WITHOUT FOCUSING ON THE PATH //
			// THEN CREATES AN INPUT FILE STREAM FOR IT. //
		fileName = fileEntry.path().stem().string() + ".py";
		std :: ifstream fileStream = std :: ifstream(APIGlobals :: workingPath + "scripts/" + fileName);

		// READS THE INPUT FILE STREAM AND GENERATES STRING STREAM DATA FROM IT //
		std :: string line; std :: stringstream execStream;
		while(std :: getline(fileStream, line))
			execStream << line << std :: endl;

		// EXECUTES CODE IN STRING STREAM //
		pybind11 :: exec(execStream.str().c_str());
		fileStream.close();
	}

	// GOES THROUGH ALL ACTIVE SCRIPTS AND RELOADS THEM //
	if(!firstTimeCompile)
		for(entityID curEnt : sceneManagement :: sceneView(globals :: curSceneRef, SCRIPT_COMP_ID))
		{
			Script * script = ((Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID][curEnt]));
			globals :: curSceneRef -> components[SCRIPT_COMP_ID][curEnt] = (compPtr) 
				scriptHandler :: createScript(script -> name, curEnt); 
		}
	firstTimeCompile = false;
}

void appManagement :: startScripts()
{
	for(entityID curEnt : sceneManagement :: sceneView(globals :: curSceneRef, SCRIPT_COMP_ID))
	{
		((Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID][curEnt])) 
			-> code.attr("_start")();
	}
}

void appManagement :: run()
{
	// VARIABLE INITIALIZATION //
	unsigned long int newDelta = SDL_GetTicks();
	unsigned long int oldDelta = 0;
	static float timer = 0.0f;

	while(globals :: isRunning)
	{
		// UPDATES DELTA TIME //
		oldDelta = newDelta;
		newDelta = SDL_GetTicks();
		
		globals :: deltaTime = (newDelta - oldDelta) / 1000.0f;

		// UPDATES ENGINE //
		appManagement :: update();

		// UPDATES SCENE THROUGH SCRIPTS //
		sceneManagement :: updateScene(globals :: curSceneRef, globals :: deltaTime);

		// BEGINS RENDERING PHASE //
		graphicManagement :: beginRenderPass();

		// UI WORK //
		uiManagement :: drawEditorUI();

		// RENDERS CURRENT 3D SCENE //
		sceneManagement :: renderScene(globals :: curSceneRef);
		
		// PRESENTS TO SCREEN //
		graphicManagement :: present();
	}
}

void appManagement :: update()
{
	// VARIABLE INITIALIZATION //
	SDL_Event event;

	// UPDATES MOUSE POSITION VALUE //
	SDL_GetMouseState(&(globals :: inputState -> mouseX), &(globals :: inputState -> mouseY));

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
				modifierKeyProcess(globals :: inputState, event, true);

				// IF ABOVE NORMAL ASCII RANGE, IGNORES //
				if(event.key.keysym.sym > 128)
					break;

				// IF READING ESCAPE KEY, QUITS //
				if(event.key.keysym.sym == 27)
				{
					globals :: isRunning = false;
					break;
				}

				if(event.key.keysym.sym == '1')
				{
					sceneManagement :: changeScene(sceneManagement :: loadScene("TEST"));
					break;
				}

				if(event.key.keysym.sym == '2')
				{
					sceneManagement :: changeScene(sceneManagement :: loadScene("TEST 2"));
					break;
				}

				// ADDS THE KEY TO THE PRESSED KEYS BITMASK //
				globals :: inputState -> pressedKeys[event.key.keysym.sym] = 1;

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
				modifierKeyProcess(globals :: inputState, event, false);

				// IF ABOVE NORMAL ASCII RANGE, IGNORES //
				if(event.key.keysym.sym > 128)
					break;

				// REMOVES THE KEY FROM THE PRESSED KEYS BITMASK //
				globals :: inputState -> pressedKeys[event.key.keysym.sym] = 0;
			break;

			case SDL_MOUSEWHEEL :
				globals :: inputState -> scroll = event.wheel.y;
			break;

			case SDL_MOUSEBUTTONDOWN :
				mouseButtonProcess(globals :: inputState, event, true);	
			break;
	
			case SDL_MOUSEBUTTONUP :
				mouseButtonProcess(globals :: inputState, event, false);	
			break;

			case SDL_MOUSEMOTION :
				globals :: inputState -> mouseDeltaX = event.motion.xrel;
				globals :: inputState -> mouseDeltaY = event.motion.yrel;
			break;

			case SDL_QUIT :
				globals :: isRunning = false;
			break;
		};
	}

	// PROCESSED INPUT //
	entityID camID;
	if(sceneManagement :: getCameraEntityID(globals :: curSceneRef, &camID))
		inputManagement :: processInput(camID);

	// RESETS RELATIVE INPUT VALUES //
	inputManagement :: resetInput(globals :: inputState);
}
