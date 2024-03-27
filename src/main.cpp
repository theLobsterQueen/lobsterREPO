/// <summary> ///
///		This file serves as the entry point to the whole program. Creates initial game
///			state data, prepares engine to accept input, and generates SDL2 window.
/// </summary> ///

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#define PY_SSIZE_T_CLEAN
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <iostream>
#include <map>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <globals.h>
#include <app.h>
#include <mesh.h>
#include <importantConstants.h>
#include <vendorIncludes.h>
#include <APIUtils.h>

int main(int argv, char ** args)
{
	// INITIALIZES ENGINE //
	appManagement :: initialize();

	// BEGINS ENGINE OPERATION //
	appManagement :: begin();

	// CLEAN UP APPPLICATION //
	appManagement :: cleanup();

	return 0;
}
