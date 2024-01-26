#ifndef LOBSTER_INPUT
#define LOBSTER_INPUT 1

// STANDARD INCLUDES //
#include <iostream>
#include <string>
#include <bitset>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>

/// <summary> ///
///		This struct controls the state of what buttons are pressed, the state of
///			control keys, and the position/delta-value of the mouse.
/// </summary> ///

struct InputState
{
	// IS A BIT MASK FOR EVERY PRESSED KEY //
	std :: bitset<128> pressedKeys = {0};

	// BOOLEANS DESCRIBING THE CURRENT VALUE OF CONTROL KEYS //
	bool cntrlPressed = false;
	bool shiftPressed = false;
	bool altPressed = false;
};


#endif
