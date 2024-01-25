// INCLUDES DEFINITION AND USES NAMESPACE //
#include <app.h>

// FUNCTION IMPLEMENTATIONS //
void app :: begin(EngineCore * core)
{
	// INITIALIZES CORE VALUES //
	core -> basePipelineRef = new Pipeline();
	core -> curSceneRef = new Scene;

	// CREATES BASE RENDER PIPELINE //
	core -> basePipelineRef -> init();
	core -> basePipelineRef -> loadShader(GL_VERTEX_SHADER, "shaders/vertShader.txt");
	core -> basePipelineRef -> loadShader(GL_FRAGMENT_SHADER, "shaders/fragShader.txt");
	core -> basePipelineRef -> linkProgram();

	// CHANGES THE NAME OF THE WINDOW TO THE NEW CURRENT SCENE //
	graphics :: changeTitle(core, core -> curSceneRef -> name);

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
		({
			-0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		})
	);
}
