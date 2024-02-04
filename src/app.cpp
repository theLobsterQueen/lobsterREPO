// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// FUNCTION IMPLEMENTATIONS //
void appManagement :: begin(EngineCore * core)
{
	// INITIALIZES CORE VALUES //
	core -> curPipelineRef = graphicManagement :: createPipeline();
	core -> curSceneRef = sceneManagement :: createScene();

	// VARIABLE INITIALIZATION //
	float aspectRatio = (core -> winWidth) / (core -> winHeight);
	core -> hozFOV = math :: toRadians(35);
	core -> verFOV = aspectRatio * core -> hozFOV;

	// PUSHES CONSTANTS //
	pushConstant("c_hozFOV", core -> hozFOV);
	pushConstant("c_verFOV", core -> verFOV);

	// CREATES BASE RENDER PIPELINE //
	graphicManagement :: loadShader
	(
		core -> curPipelineRef, GL_VERTEX_SHADER, 
		"shaders/vertShader.txt", core -> debug
	);
	graphicManagement :: loadShader
	(
		core -> curPipelineRef, GL_FRAGMENT_SHADER, 
		"shaders/fragShader.txt", core -> debug
	);

	graphicManagement :: compileProgram(core -> curPipelineRef);

	// CHANGES THE NAME OF THE WINDOW TO THE NEW CURRENT SCENE //
	windowManagement :: changeTitle(core -> winRef, core -> curSceneRef -> name);

	// (TEMP) CREATES TEST SCENE //
	createTestScene(core);

	// BEGINS RUNNING THE ENGINE //
	run(core);
}

void appManagement :: createTestScene(EngineCore * core)
{
	// SETS THE PLAYER'S STARTING POSITION //
	// core -> curSceneRef -> viewRef -> playerPos[2] = -2000.0f;

	sceneManagement :: addComp
	(
		core -> curSceneRef,
		sceneManagement :: newEntityID(core -> curSceneRef),
		componentManagement :: getID<Mesh>(),
		(compPtr) meshManagement :: createMesh
		(
			// VERTICES //
			std :: vector<GLfloat>
			{
				// FRONT-FACE //
				-1.0f, 1.0f, -0.1f,
				1.0f, 1.0f, -0.1f,
				1.0f, -1.0f, -0.1f,
				-1.0f, -1.0f, -0.1f,
				
				// BACK-FACE //
				-1.0f, 1.0f, 0.1f,
				1.0f, 1.0f, 0.1f,
				1.0f, -1.0f, 0.1f,
				-1.0f, -1.0f, 0.1f,
			},

			// INDICES //
			std :: vector<GLuint>
			{
				// FRONT FACE //
				0, 1, 2,
				2, 3, 0,

				// LEFT FACE //
				0, 4, 3,
				3, 7, 4,

				// BACK FACE //
				4, 5, 6,
				6, 7, 4,

				// RIGHT FACE //
				1, 5, 6,
				6, 2, 1
			},

			// COLORS //
			std :: vector<GLfloat>
			{
				0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,


				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f
			}
		)
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
		sceneManagement :: renderScene(core);

		// PRESENTS TO SCREEN //
		graphicManagement :: present(core);
	}
}

void appManagement :: update(EngineCore * core)
{
	// VARIABLE INITIALIZATION //
	SDL_Event event;
	static Scene * alternateScene = sceneManagement :: createScene("Scene2");
	GLfloat * playerPos = core -> curSceneRef -> viewRef -> playerPos;

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
				{
					// SWAPS THE ADDRESS BETWEEN STATIC ALTERNATE SAVED SCENE //
						// AND ENGINE'S CURRENT SCENE //
					Scene * tempScene = core -> curSceneRef;
					sceneManagement :: changeScene(core, alternateScene);
					alternateScene = tempScene;
				}
			}
		}
	}

	// TAKES PLAYER WASD MOVEMENT //
	float moveSpeed = 1.0f * core -> deltaTime;
	auto keys = core -> inputState -> pressedKeys;

	if(keys['a'])
		playerPos[0] -= moveSpeed;
	if(keys['d'])
		playerPos[0] += moveSpeed;
	if(keys['w'])
		playerPos[2] += moveSpeed * 40;
	if(keys['s'])
		playerPos[2] -= moveSpeed * 40;
	if(keys[','])
		playerPos[1] -= moveSpeed;
	if(keys['.'])
		playerPos[1] += moveSpeed;

	if(keys['q'])
		core -> tempScale += (moveSpeed * 10) * core -> deltaTime;
	if(keys['e'])
		core -> tempScale -= (moveSpeed * 10) * core -> deltaTime;

	float rotSpeed = moveSpeed * 20;
	if(keys['r'])
		core -> tempRot[0] += (rotSpeed * core -> deltaTime);
	if(keys['t'])
		core -> tempRot[0] -= (rotSpeed * core -> deltaTime);

	if(keys['y'])
		core -> tempRot[1] += (rotSpeed * core -> deltaTime);
	if(keys['u'])
		core -> tempRot[1] -= (rotSpeed * core -> deltaTime);

	if(keys['i'])
		core -> tempRot[2] += (rotSpeed * core -> deltaTime);
	if(keys['o'])
		core -> tempRot[2] -= (rotSpeed * core -> deltaTime);

	if(core -> tempScale < 0.05f)
		core -> tempScale = 0.05f;
}
