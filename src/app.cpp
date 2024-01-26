// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// FUNCTION IMPLEMENTATIONS //
void appManagement :: begin(EngineCore * core)
{
	// INITIALIZES CORE VALUES //
	core -> basePipelineRef = graphicManagement :: createPipeline();
	core -> curSceneRef = sceneManagement :: createScene();

	// CREATES BASE RENDER PIPELINE //
	graphicManagement :: loadShader
		(core -> basePipelineRef, GL_VERTEX_SHADER, "shaders/vertShader.txt");
	graphicManagement :: loadShader
		(core -> basePipelineRef, GL_FRAGMENT_SHADER, "shaders/fragShader.txt");
	graphicManagement :: compileProgram(core -> basePipelineRef);

	// CHANGES THE NAME OF THE WINDOW TO THE NEW CURRENT SCENE //
	windowManagement :: changeTitle(core -> winRef, core -> curSceneRef -> name);

	// (TEMP) CREATES TEST SCENE //
	createTestScene(core);

	// BEGINS RUNNING THE ENGINE //
	run(core);
}

void appManagement :: run(EngineCore * core)
{
	// VARIABLE INITIALIZATION //
	unsigned long int newDelta = SDL_GetTicks64();
	unsigned long int oldDelta = 0;
	
	// MAIN OPERATION LOOP //
	while(core -> isRunning)
	{
		// UPDATES DELTA TIME //
		oldDelta = newDelta;
		newDelta = SDL_GetTicks64();
		
		core -> deltaTime = (newDelta - oldDelta) * (30 / 1000.0f);

		// UPDATES GAME STATE //
		appManagement :: update(core);

		// RENDER PHASE //
		graphicManagement :: beginRenderPass(0.1f, 0.1f, 0.1f, 1.0f);
		
		// RENDERS THE CURRENT SCENE //
		sceneManagement :: renderScene(core -> curSceneRef, core -> basePipelineRef);
		
		// PRESENTS THE RENDERED PRODUCT TO THE USER //
		graphicManagement :: present(core);
	}
}

void appManagement :: createTestScene(EngineCore * core) 
{
	// CREATE TRANS ICON BECAUSE I'M JUST THAT DAMN COOL //
	sceneManagement :: addComp
	(
		core -> curSceneRef, // TARGET SCENE TO ADD COMPONENT TO //
		sceneManagement :: newEntityID(core -> curSceneRef), // ENTITY INDEX (EMPTY ENTITY) //
		componentManagement :: getID<Mesh>(), // ID OF COMPONENT TO ADD //
		(compPtr) meshManagement :: createMesh
		(
			// POSITION VEC //
			{
				 0.8f,  0.0f, 0.0f,
				 0.1f,  0.8f, 0.0f,
				 0.1f, -0.8f, 0.0f
			},

			// COLOR VEC //
			{
				1.0f, 0.0f, 0.5f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f
			}
		)
	);

	sceneManagement :: addComp
	(
		core -> curSceneRef, // TARGET SCENE TO ADD COMPONENT TO //
		sceneManagement :: newEntityID(core -> curSceneRef), // ENTITY INDEX (EMPTY ENTITY) //
		componentManagement :: getID<Mesh>(), // ID OF COMPONENT TO ADD //
		(compPtr) meshManagement :: createMesh
		(
			// POSITION VEC //
			{
				-0.8f,  0.0f, 0.0f,
				-0.1f,  0.8f, 0.0f,
				-0.1f, -0.8f, 0.0f
			},

			// COLOR VEC //
			{
				0.0f, 0.5f, 1.0f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f,
				0.8f, 0.8f, 0.8f, 1.0f
			}
		)
	);

	// PLAYER RETICLE //
	GLuint retID = sceneManagement :: newEntityID(core -> curSceneRef);
	Transform * retTrans = new Transform;
	retTrans -> isCentered = true;

	sceneManagement :: addComp
	(
		core -> curSceneRef, // TARGET SCENE TO ADD COMPONENT TO //
		retID, // ENTITY INDEX (PLAYER RETICLE) //
		componentManagement :: getID<Mesh>(), // ID OF COMPONENT TO ADD //
		(compPtr) meshManagement :: createMesh
		(
			// POSITION VEC //
			{
				 0.0f , -0.05f, 0.0f ,
				-0.05f,  0.0f , 0.0f ,
				 0.0f ,  0.05f, 0.0f ,

				 0.0f ,  0.05f, 0.0f ,
				 0.05f,  0.0f , 0.0f ,
				 0.0f , -0.05f, 0.0f 
			},

			// COLOR VEC //
			{
				0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f, 1.0f,

				0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			}
		)
	);
	sceneManagement :: addComp
	(
		core -> curSceneRef, 
		retID, 
		componentManagement :: getID<Transform>(), 
		(compPtr) retTrans 
	);
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
					sceneManagement :: useScene(core, alternateScene);
					alternateScene = tempScene;
				}
			}
		}
	}

	// TAKES PLAYER WASD MOVEMENT //
	float moveSpeed = 0.05f * core -> deltaTime;
	auto keys = core -> inputState -> pressedKeys;

	if(keys['a'])
		playerPos[0] -= moveSpeed;
	if(keys['d'])
		playerPos[0] += moveSpeed;
	if(keys['w'])
		playerPos[1] += moveSpeed;
	if(keys['s'])
		playerPos[1] -= moveSpeed;
}
