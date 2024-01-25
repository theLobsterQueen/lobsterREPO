#ifndef LOBSTER_GRAPHICS_H
#define LOBSTER_GRAPHICS_H

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

// PROTOTYPE STRUCTS //
struct EngineCore;

/// <summary> ///
///		This file defines functions that change the appearence of the engine in some way,
///			but do not fit within other objects such as the render pipeline
///			or individual meshes.
/// </summary> ///

namespace graphics
{
	// FUNCTION DEFINITIONS //

	/// <summary> ///
	///		Changes the title of the main window.
	/// </summary> ///
	void changeTitle(EngineCore * core, std :: string windowName);

	/// <summary> ///
	///		This function clears the screen a specific color, and prepares the
	///			engine for another render pass.
	/// </summary> ///
	void beginRenderPass(float r, float g, float b, float a);

	/// <summary> ///
	///		Flips the buffers over using a more engine-friendly function
	///			than the lower level SDL2 function.
	/// </summary> ///
	void present(EngineCore * core);
};

#endif
