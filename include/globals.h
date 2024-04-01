#ifndef LOBSTER_ENGINE_CORE
#define LOBSTER_ENGINE_CORE 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <sstream>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <vendorIncludes.h>
#include <options.h>

// STRUCT PROTOTYPES //
struct Scene;
struct Pipeline;
struct InputState;

/// <summary> ///
///		This namespace includes value crucial to keeping the engine functional at the most
///			fundamental level. 
/// </summary> ///

namespace globals
{
	/// <summary> ///
	///		If this boolean is ever set to false, turns off the application.
	/// </summary> ///

	extern bool isRunning;
	extern bool isPlaying;

	/// <summary> ///
	/// 	This pointer references the main window.
	/// </summary> ///

	extern SDL_Window * winRef;

	/// <summary> ///
	///		These values dictate the starting width/height of the engine window.
	///			(Defaults to standard HD 16:9 resolution.)
	/// </summary> ///

	extern unsigned short int winWidth;
	extern unsigned short int winHeight;

	/// <summary> ///
	///		This pointer references the OpenGL context.
	/// </summary> ///

	extern void * glRef;

	/// <summary> ///
	///		This field contains the currently actively scene.
	/// </summary> ///

	extern Scene * curSceneRef;

	/// <summary> ///
	///		This field contains a reference to the general input state
	///			of the application. 
	/// </summary> ///

	extern InputState * inputState;



	/// <summary> ///
	///		A collection of all created pipelines within the engine context.
	/// </summary> ///

	extern std :: vector<Pipeline *> pipelineRefs;

	/// <summary> ///
	///		This field holds the time passed since the last time getDeltaTime was
	///			called. getDeltaTime can be found in the appManagement namespace.
	/// </summary> ///

	extern double deltaTime;

	/// <summary> ///
	///		The color that the screen is automatically cleared to at the beginning
	///			of each frame.
	/// </summary> ///

	extern float clearColor[4];
	extern float tempScale;
	extern std :: vector<float> tempRot;
};

namespace editorGlobals
{
	/// <summary> ///
	///		These flags define the baseline behavior for different types of UI
	///			objects. 
	/// </summary> ///

	extern ImGuiWindowFlags 	windowFlags;
	extern ImGuiTreeNodeFlags 	sceneTreeFlags;
	extern ImGuiInputTextFlags	inputTextFlags;

	/// <summary> ///
	///		This bool describes whether or not the user is currently entering key
	///			input to a UI item. If so, it disables camera movement.
	/// </summary> ///

	extern bool keyInput;

	/// <summary> ///
	///		This ID represents the entity currently selected within the scene hiearchy.
	/// </summary> ///

	extern entityID curActiveEntity;
	extern bool entitySelected;

	/// <summary> ///
	///		These values describe the parameters in which the editor frontend is organized.
	/// </summary> ///

	extern float sidePanelWidth;
	extern float sidePanelHeight;

	extern float bottomPanelWidth;
	extern float bottomPanelHeight;

	extern std :: stringstream debugText;

	/// <summary> ///
	///		This field contains a reference to an instance of the "Options" struct,
	///			defining some general parameters for the behavior of the editor.
	/// </summary> ///

	extern Options * optionsRef;
};

#endif
