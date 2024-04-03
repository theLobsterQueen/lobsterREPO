// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// FUNCTION IMPLEMENTATIONS //
void appManagement :: initialize()
{
	// INITIALIZES GLOBALS //
	globals :: isRunning = true;
	globals :: isPlaying = false;
	globals :: winWidth = 1600;
	globals :: winHeight = 900;
	globals :: deltaTime = 0;

	globals :: clearColor[0] = 0.1f;
	globals :: clearColor[1] = 0.1f;
	globals :: clearColor[2] = 0.4f;
	globals :: clearColor[3] = 0.0f;

	editorGlobals :: keyInput = false;
	editorGlobals :: windowFlags = 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	editorGlobals :: sceneTreeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	editorGlobals :: inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue |
		ImGuiInputTextFlags_AutoSelectAll;
	editorGlobals :: entitySelected = false;
	editorGlobals :: sidePanelWidth = (globals :: winWidth) * 0.2f;
	editorGlobals :: sidePanelHeight = (globals :: winHeight) * 0.7f;
	editorGlobals :: bottomPanelWidth = globals :: winWidth - editorGlobals :: sidePanelWidth;
	editorGlobals :: bottomPanelHeight = globals :: winHeight - editorGlobals :: sidePanelHeight;
	editorGlobals :: debugText = std :: stringstream("");
	editorGlobals :: optionsRef = new Options;

	// INITIALIZES SDL //
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std :: cout << std :: endl << SDL_GetError() << std :: endl;
		return;
	}

	// SETS OPEN-GL ATTRIBUTES //
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// SETS BIT SIZE = 32 RGBA //
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	// ENABLES DOUBLE-BUFFERING //
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	// ENABLES DEPTH BUFFER TESTING AND  INCREASES DEPTH BUFFER SIZE //
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// FORCES HARDWARE ACCELERATION //
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// CREATES WINDOW AND READS OPEN-GL CONTEXT //
	int SDL_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_CAPTURE;
	globals :: winRef = SDL_CreateWindow
	(
		"Lobster Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		globals :: winWidth, globals :: winHeight, SDL_FLAGS
	);

	globals :: glRef = SDL_GL_CreateContext(globals :: winRef);
	if(globals :: glRef == nullptr)
	{
		std :: cout << "ERROR! OPEN-GL CONTEXT COULD NOT BE CREATED: " << SDL_GetError()
			<< std :: endl;
		return;
	}

	// CODE TAKEN FROM img_impl_opengl3.h //

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	// ENABLES KEYBOARD CONTROLS //
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(globals :: winRef, globals :: glRef);
	ImGui_ImplOpenGL3_Init("#version 430");

	// INITIALIZES GLEW //
	glewExperimental = GL_TRUE;
	glewInit();

	// SETS OUT OPEN-GL SETTINGS //
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// SETS OUT STB SETTINGS //
	stbi_set_flip_vertically_on_load(true);

	// PRINTS OUT OPEN-GL VERSION //
	std :: cout << "GL VENDOR: " << glGetString(GL_VENDOR) << std :: endl;
	std :: cout << "GL RENDERER: " << glGetString(GL_RENDERER) << std :: endl;
	std :: cout << "GL VERSION: " << glGetString(GL_VERSION) << std :: endl;
	std :: cout << "GLSL VERSION: " 
		<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std :: endl;
}

void appManagement :: cleanup()
{
	// CLEAN UP APPPLICATION //
	pybind11 :: finalize_interpreter();
	SDL_DestroyWindow(globals :: winRef);
	SDL_Quit();
}

void appManagement :: begin(std :: string sceneLoad)
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

	// CREATES BASE SCENE //
	Scene * startScene;
	if(sceneLoad == "")
		startScene = appManagement :: createBaseScene();
	else
		startScene = sceneManagement :: loadScene(sceneLoad);
	sceneManagement :: changeScene(startScene);

	// BEGINS RUNNING THE ENGINE //
	appManagement :: run();
}

Scene * appManagement :: createBaseScene()
{
	// CREATES CAMERA COMPONENT //
	Scene * testScene = sceneManagement :: createScene("New Scene");
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

	// RETURNS THE NEWLY CREATED SCENE //
	return testScene;
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
	pybind11 :: exec("import inspect");
	pybind11 :: exec("import coremodule");
	
	APIGlobals :: coremodule = pybind11 :: module_ :: import("coremodule");
	APIGlobals :: inputmodule = pybind11 :: module_ :: import("inputmodule");
	APIGlobals :: appmodule = pybind11 :: module_ :: import("appmodule");
}

void appManagement :: compileScripts()
{
	// VARIABLE INITIALIZATION //
	static bool firstTimeCompile = true;

	// FINDS SCRIPT DIRECTORY //
	std :: filesystem :: directory_iterator dirIt = std :: filesystem :: begin
		(std :: filesystem :: directory_iterator(std :: string
		(APIGlobals :: workingPath + "scripts/").c_str())); 

	// RUNS ALL SCRIPTS IN THE SCRIPT DIRECTORY //
	unsigned scriptsCompiled = 0;
	APIGlobals :: coremodule.attr("script_refs").attr("clear")();
	for(std :: filesystem :: directory_entry fileEntry : dirIt)
	{
		// DETERMINES WHETHER THE CURRENT FILE IS A PYTHON FILE //
		std :: string fileName = fileEntry.path().string();
		if(fileName.find(".py") == std :: string :: npos ||
			fileName.find(".swp") != std :: string :: npos)
		{
			continue;
		}

		// ICNREMENTS THE SCRIPT COMPILED COUNTER, SO PROGRAM CAN TELL DEVELOPER HOW MANY //
			// SCRIPTS IT FOUND //
		scriptsCompiled++;

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
		APIGlobals :: coremodule.attr("script_refs").attr("clear")();
		pybind11 :: exec
		(
		 	"dir = globals().copy()\nfor item in dir :\n\
			\titem_script = globals()[item]\n\
			\tif inspect.isclass(item_script) and item_script not in coremodule.script_refs :\n\
			\t\tcoremodule.script_refs[item] = item_script"
		);
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

	// REPORTS HOW MANY SCRIPTS WERE FOUND //
	std :: cout << "FOUND AND COMPILED " << scriptsCompiled << " SCRIPT(S)!" << std :: endl;
}

void appManagement :: startScripts()
{
	// ADDS ALL CURRENTLY EXISTING COMPONENTS INTO THE API //
	APIGlobals :: coremodule.attr("clear_comps")();
	unsigned curActive = globals :: curSceneRef -> activeEntities;
	for(entityID curEntID = 0; curEntID < LOBSTER_MAX_ENTITIES && curActive > 0; curEntID++)
	{
		Entity curEnt = globals :: curSceneRef -> entities[curEntID];
		if(curEnt.mask == 0)
			continue;

		curActive--;
		Transform * transData = (Transform *) 
			(globals :: curSceneRef -> components[TRANS_COMP_ID][curEntID]);
		APIGlobals :: coremodule.attr("Transform")
			(curEntID, transData -> position, transData -> rotation, transData -> scale);

		if(hasComp(curEnt, MESH_COMP_ID))
		{
			Mesh * meshData = (Mesh *) (globals :: curSceneRef -> components[MESH_COMP_ID][curEntID]);
			APIGlobals :: coremodule.attr("Mesh")(curEntID, meshData -> name, meshData -> texName);
		}

		if(hasComp(curEnt, SCRIPT_COMP_ID))
		{
			Script * scriptData = 
				((Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID][curEntID]));
			APIGlobals :: coremodule.attr("Script")(curEntID, scriptData -> name);
		}

		if(hasComp(curEnt, LIGHT_COMP_ID))
		{
			Light * lightData = (Light *) (globals :: curSceneRef -> components[LIGHT_COMP_ID][curEntID]);
			APIGlobals :: coremodule.attr("Light")(curEntID, lightData -> color);
		}
	}
	
	// CLEARS ORDERS AFTER INITIALIZING SCENE IN ORDER TO STOP DUPLICATE INITIALIZATIONS //
	APIGlobals :: coremodule.attr("clear_orders")();

	// RUNS THE AWAKE SCRIPTS, AND ADDS ANY ENTITIES/COMPONENTS TO THE SCENE THAT WERE CREATED //
	for(entityID curEnt : sceneManagement :: sceneView(globals :: curSceneRef, SCRIPT_COMP_ID))
		((Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID][curEnt]))
			-> code.attr("_awake")(); 

	// PULLS AND PROCESS ORDERS FROM AWAKE FUNCTIONS //
	sceneManagement :: pullOrders(globals :: curSceneRef);

	// RUNS THE START SCRIPT //
	for(entityID curEnt : sceneManagement :: sceneView(globals :: curSceneRef, SCRIPT_COMP_ID))
		((Script *) (globals :: curSceneRef -> components[SCRIPT_COMP_ID][curEnt])) 
			-> code.attr("_start")();
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
		if(globals :: isPlaying)
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
	// DETERMINES WHETHER OR NOT THE ENGINE SHOULD YIELD //
		// KEY INPUT TO THE IMGUI LAYER //
	editorGlobals :: keyInput = ImGui :: GetIO().WantCaptureKeyboard;

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
					if(globals :: isPlaying)
					{
						globals :: isPlaying = false;
						sceneManagement :: changeScene(sceneManagement :: loadScene
								(globals :: curSceneRef -> name + ".lscn"));
					}

					else
						globals :: isRunning = false;
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
