#ifndef LOBSTER_WINDOW_H
#define LOBSTER_WINDOW_H 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <string>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <engineCore.h>

/// <summary> ///
///		This file defines functions that change the appearence of the engine in some way,
///			but do not fit within other objects such as the render pipeline
///			or individual meshes.
/// </summary> ///

namespace windowManagement
{
	// FUNCTION DEFINITIONS //

	/// <summary> ///
	///		Changes the title of the main window.
	/// </summary> ///
	void changeTitle(SDL_Window * window, std :: string windowName);
};

#endif
