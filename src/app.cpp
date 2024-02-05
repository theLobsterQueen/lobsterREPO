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
	entityID newEntity = sceneManagement :: newEntityID(core -> curSceneRef);
	sceneManagement :: addComp
	(
		core -> curSceneRef,
		newEntity,
		MESH_COMP_ID,
		(compPtr) meshHandler :: createMesh
		(
			// VERTICES //
			std :: vector<GLfloat>
			{
				// FRONT-FACE //
				-1.0f, 1.0f, -0.5f,
				1.0f, 1.0f, -0.5f,
				1.0f, -1.0f, -0.5f,
				-1.0f, -1.0f, -0.5f,
				
				// BACK-FACE //
				-1.0f, 1.0f, 0.5f,
				1.0f, 1.0f, 0.5f,
				1.0f, -1.0f, 0.5f,
				-1.0f, -1.0f, 0.5f,
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

	sceneManagement :: addComp
	(
		core -> curSceneRef,
		newEntity,
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

			}
		}
	}
}
