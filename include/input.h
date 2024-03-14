#ifndef LOBSTER_INPUT
#define LOBSTER_INPUT 1

// STANDARD INCLUDES //
#include <iostream>
#include <string>
#include <bitset>

// LOBSTER INCLUDES //
#include <importantConstants.h>
#include <engineCore.h>
#include <entity.h>
#include <camera.h>
#include <utilities.h>
#include <scene.h>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>

// STRUCT PROTOTYPES //
struct EngineCore;

/// <summary> ///
///		This struct controls the state of what buttons are pressed, the state of
///			control keys, and the position/delta-value of the mouse.
/// </summary> ///

struct InputState
{
	// IS A BIT MASK FOR EVERY PRESSED KEY //
	std :: bitset<128> pressedKeys = {0};

	// HOLDS THE CURRENT POSITION OF THE MOUSE //
	int mouseDeltaX = 0; int mouseDeltaY = 0; 
	int mouseX = 0; int mouseY = 0;
	int scroll = 0; bool mmb = false; bool lmb = false; bool rmb = false;

	// BOOLEANS DESCRIBING THE CURRENT VALUE OF CONTROL KEYS //
	bool cntrlPressed = false;
	bool shiftPressed = false;
	bool altPressed = false;
};

namespace inputManagement
{
	///	<summary> ///
	///		Creates an input state pointer and returns it.
	/// </summary> ///

	InputState * createInputState();

	/// <summary> ///
	///		Resets the value of the inptu state. This is used to ensure that
	///			values which relative values (like mouseX/mouseY) are continually
	///			reset so that input processing works as intended.
	/// </summary> ///

	void resetInput(InputState * input);

	/// <summary> ///
	///		Processes user input to manipulate the position and rotation of the
	///			viewport camera.
	///		TODO: Add support for selecting entities, using shortcuts, etc.
	/// </summary> ///

	void processInput(EngineCore * core, entityID cameraID);
};

#endif
