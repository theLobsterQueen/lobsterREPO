#ifndef LOBSTER_APP
#define LOBSTER_APP 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <engineCore.h>
#include <editorData.h>
#include <window.h>
#include <mesh.h>
#include <scene.h>
#include <transform.h>
#include <utilities.h>
#include <importantConstants.h>
#include <math.h>
#include <light.h>
#include <vendorIncludes.h>
#include <ui.h>
#include <globals.h>
#include <APIUtils.h>

namespace appManagement
{
	// FUNCTION DEFINITIONS //

	/// <summary> ///
	///		This setups a base empty scene.
	/// </summary> ///
	void begin(EngineCore * core);

	/// <summary> ///
	///		Sets up the working directory, initializes the pybind11 interpreter, and
	///			prepares the Python context.
	/// </summary> ///

	void initializeAPI(EngineCore * core);

	/// <summary> ///
	///		Iterates through all scripts in the "script" directory, and executes their code.
	/// </summary> ///

	void compileScripts(EngineCore * core);

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

	/// <summary> ///
	///		Updates the general game state by processing SDL2 events.
	/// </summary> ///

	void update(EngineCore * core);
};

#endif
