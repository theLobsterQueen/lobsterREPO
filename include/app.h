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
#include <globals.h>
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
#include <APIUtils.h>
#include <script.h>

// STRUCT PROTOTYPES //
struct Scene;

namespace appManagement
{
	// FUNCTION DEFINITIONS //

	/// <summary> ///
	///		Does all the initialization/cleanup work that main.cpp used to do.
	/// </summary> ///

	void initialize();
	void cleanup();

	/// <summary> ///
	///		This setups a base empty scene.
	/// </summary> ///
	void begin(std :: string sceneLoad = "");

	/// <summary> ///
	///		Sets up the working directory, initializes the pybind11 interpreter, and
	///			prepares the Python context.
	/// </summary> ///

	void initializeAPI();

	/// <summary> ///
	///		Iterates through all scripts in the "script" directory, and executes their code.
	/// </summary> ///

	void compileScripts();

	/// <summary> ///
	///		Calls the "_start" function on all scripts currently in
	///			the scene.
	/// </summary> ///

	void startScripts();

	/// <summary> ///
	///		When called, puts the engine into the "control loop." This loop will
	///			continually parse input, render meshes, and so on, until the program
	///			shuts down. 
	/// </summary> ///

	void run();

	/// <summary> ///
	///		This populates the editor with a base screen.
	/// </summary> ///

	Scene * createBaseScene();

	/// <summary> ///
	///		Updates the general game state by processing SDL2 events.
	/// </summary> ///

	void update();
};

#endif
