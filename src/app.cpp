// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// FUNCTION IMPLEMENTATIONS //
void app :: begin(EngineCore * core)
{
	// INITIALIZES CORE VALUES //
	core -> basePipelineRef = graphics :: createPipeline();
	core -> curSceneRef = sceneManagement :: createScene();

	// CREATES BASE RENDER PIPELINE //
	graphics :: loadShader
		(core -> basePipelineRef, GL_VERTEX_SHADER, "shaders/vertShader.txt");
	graphics :: loadShader
		(core -> basePipelineRef, GL_FRAGMENT_SHADER, "shaders/fragShader.txt");
	graphics :: compileProgram(core -> basePipelineRef);

	// CHANGES THE NAME OF THE WINDOW TO THE NEW CURRENT SCENE //
	windowManagement :: changeTitle(core -> winRef, core -> curSceneRef -> name);

	// (TEMP) CREATES TEST SCENE //
	createTestScene(core);

	// BEGINS RUNNING THE ENGINE //
	run(core);
}

void app :: run(EngineCore * core)
{
	// VARIABLE INITIALIZATION //
	bool isRunning = true;

	// MAIN OPERATION LOOP //
	while(isRunning)
	{
		// RENDER PHASE //
		graphics :: beginRenderPass(0.1f, 0.1f, 0.1f, 1.0f);
		
		// RENDERS THE CURRENT SCENE //
		sceneManagement :: renderScene(core -> curSceneRef, core -> basePipelineRef);
		
		// PRESENTS THE RENDERED PRODUCT TO THE USER //
		graphics :: present(core);
	}
}

void app :: createTestScene(EngineCore * core) 
{
	// ADDS A MESH COMPONENT TO THE FIRST AVAILABLE ENTITY //
	sceneManagement :: addComp
	(
		core -> curSceneRef, // TARGET SCENE TO ADD COMPONENT TO //
		sceneManagement :: newEntityID(core -> curSceneRef), // ENTITY INDEX (EMPTY ENTITY) //
		componentManagement :: getID<Mesh>(), // ID OF COMPONENT TO ADD //
		(char *) mesh :: createMesh
		(
			// POSITION VEC //
			{
				-0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f
			},

			// COLOR VEC //
			{
				1.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f
			}
		)
	);
}
