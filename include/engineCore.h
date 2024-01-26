#ifndef LOBSTER_ENGINE_CORE
#define LOBSTER_ENGINE_CORE 1

// INCLUDES AND INITIALIZATIONS //

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <input.h>

// STRUCT PROTOTYPES //
struct Scene;
struct Pipeline;

/// <summary> ///
///		This struct includes value crucial to keeping the engine functional at the most
///			fundamental level. 
/// </summary> ///

struct EngineCore
{
	/// <summary> ///
	///		If this boolean is ever set to false, turns off the application.
	/// </summary> ///
	bool isRunning = true;

	/// <summary> ///
	///		This boolean describes whether the engine should give verbose
	///			debug statements during its operation.
	/// </summary> ///
	bool debug = false;

	/// <summary> ///
	/// 	This pointer references the main window.
	/// </summary> ///
	SDL_Window * winRef = nullptr;

	/// <summary> ///
	///		These values dictate the starting width/height of the engine window.
	///			(Defaults to standard HD 16:9 resolution.)
	/// </summary> ///
	unsigned short int winWidth = 1280;
	unsigned short int winHeight = 720;

	/// <summary> ///
	///		This pointer references the OpenGL context.
	/// </summary> ///
	void * glRef = nullptr;

	/// <summary> ///
	///		This is a reference to the "base pipeline," or the pipeline that all
	///			projects in this engine initialize with.
	/// </summary> ///
	Pipeline * basePipelineRef = nullptr;


	/// <summary> ///
	///		This field contains the currently actively scene.
	/// </summary> ///
	Scene * curSceneRef = nullptr;

	/// <summary> ///
	///		This field contains a reference to the general input state
	///			of the application. 
	/// </summary> ///
	InputState * inputState = new InputState;

	/// <summary> ///
	///		This field holds the time passed since the last time getDeltaTime was
	///			called. getDeltaTime can be found in the appManagement namespace.
	/// </summary> ///
	double deltaTime = 0;
};

#endif
