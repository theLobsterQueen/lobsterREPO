#ifndef LOBSTER_APP
#define LOBSTER_APP 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <iostream>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <engineCore.h>
#include <graphics.h>
#include <mesh.h>
#include <scene.h>

namespace app
{
	// FUNCTION DEFINITIONS //

	/// <summary> ///
	///		This setups a base empty scene.
	/// </summary> ///
	void begin(EngineCore * core);

	/// <summary> ///
	///		When called, puts the engine into the "control loop." This loop will
	///			continually parse input, render meshes, and so on, until the program
	///			shuts down. 
	/// </summary> ///
	void run(EngineCore * core);

	/// <summary> ///
	///		This populates the current scene with test draw meshes.
	/// </summary> ///
	void createTestScene(EngineCore * core);
};

#endif
