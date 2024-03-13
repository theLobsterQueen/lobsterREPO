#ifndef LOBSTER_ENGINE_CORE
#define LOBSTER_ENGINE_CORE 1

// INCLUDES AND INITIALIZATIONS //

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// STD INCLUDES //
#include <vector>

// LOBSTER INCLUDES //
#include <input.h>
#include <editorData.h>

// STRUCT PROTOTYPES //
struct Scene;
struct Pipeline;
struct InputState;

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
	// </summary> ///
	bool debug = true;

	/// <summary> ///
	/// 	This pointer references the main window.
	/// </summary> ///

	SDL_Window * winRef = nullptr;

	/// <summary> ///
	///		These values dictate the starting width/height of the engine window.
	///			(Defaults to standard HD 16:9 resolution.)
	/// </summary> ///

	unsigned short int winWidth = 1300;
	unsigned short int winHeight = 650;

	/// <summary> ///
	///		This pointer references the OpenGL context.
	/// </summary> ///

	void * glRef = nullptr;

	/// <summary> ///
	///		This field contains the currently actively scene.
	/// </summary> ///

	Scene * curSceneRef = nullptr;

	/// <summary> ///
	///		This field contains a reference to the general input state
	///			of the application. 
	/// </summary> ///

	InputState * inputState = nullptr;

	/// <summary> ///
	///		A collection of all created pipelines within the engine context.
	/// </summary> ///

	std :: vector<Pipeline *> pipelineRefs;

	/// <summary> ///
	///		This field holds the time passed since the last time getDeltaTime was
	///			called. getDeltaTime can be found in the appManagement namespace.
	/// </summary> ///

	double deltaTime = 0;

	/// <summary> ///
	///		The color that the screen is automatically cleared to at the beginning
	///			of each frame.
	/// </summary> ///

	float clearColor[4] = { 0.1f, 0.1f, 0.4f, 0.0f };
	float tempScale = 1.0f;
	std :: vector<float> tempRot = { 0, 0, 0 };

	/// <summary> ///
	///		Data for the editor frontend itself.
	/// </summary> ///

	EditorData * editorDataRef = new EditorData;
};

#endif
