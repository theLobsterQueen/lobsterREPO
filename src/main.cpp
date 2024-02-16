/// <summary> ///
///		This file serves as the entry point to the whole program. Creates initial game
///			state data, prepares engine to accept input, and generates SDL2 window.
/// </summary> ///

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
#include <app.h>
#include <mesh.h>
#include <importantConstants.h>

int main(int argv, char ** args)
{
	// CREATES ENGINE CORE REFERENCE //
	EngineCore * core = new EngineCore;

	// IF DEBUG IS ENABLED, PRINTS OUT MESSAGE //
	if(core -> debug)
		std :: cout << "[DEBUG ENABLED]" << std :: endl;

	// INITIALIZES SDL //
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std :: cout << std :: endl << SDL_GetError() << std :: endl;
		return -1;
	}

	// SETS OPEN-GL ATTRIBUTES //
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// SETS BIT SIZE = 32 RGBA //
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	// ENABLES DOUBLE-BUFFERING //
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	// ENABLES DEPTH BUFFER TESTING AND  INCREASES DEPTH BUFFER SIZE //
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// FORCES HARDWARE ACCELERATION //
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// CREATES WINDOW AND READS OPEN-GL CONTEXT //
	int SDL_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_CAPTURE;
	core -> winRef = SDL_CreateWindow
	(
		"Lobster Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		core -> winWidth, core -> winHeight, SDL_FLAGS
	);

	core -> glRef = SDL_GL_CreateContext(core -> winRef);
	if(core -> glRef == nullptr)
	{
		std :: cout << "ERROR! OPEN-GL CONTEXT COULD NOT BE CREATED: " << SDL_GetError()
			<< std :: endl;
		return -1;
	}

	// INITIALIZES GLEW //
	glewExperimental = GL_TRUE;
	glewInit();

	// SETS OUT OPEN-GL SETTINGS //
	glEnable(GL_DEPTH_TEST);

	// PRINTS OUT OPEN-GL VERSION //
	std :: cout << "GL VENDOR: " << glGetString(GL_VENDOR) << std :: endl;
	std :: cout << "GL RENDERER: " << glGetString(GL_RENDERER) << std :: endl;
	std :: cout << "GL VERSION: " << glGetString(GL_VERSION) << std :: endl;
	std :: cout << "GLSL VERSION: " 
		<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std :: endl;

	// BEGINS ENGINE OPERATION //
	appManagement :: begin(core);

	// CLEAN UP APPPLICATION //
	SDL_DestroyWindow(core -> winRef);
	SDL_Quit();

	return 0;
}
