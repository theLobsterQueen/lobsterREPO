#ifndef LOBSTER_UI_H 
#define LOBSTER_UI_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <filesystem>
#include <cstdio>

// LOBSTER INCLUDES //
#include <globals.h>
#include <vendorIncludes.h>
#include <importantConstants.h>
#include <entity.h>
#include <transform.h>
#include <scene.h>
#include <app.h>

namespace uiManagement
{
	/// <summary> ///
	///		This file draws the basic overlay for the editor, containing
	///			data from the current scene.
	/// </summary> ///

	void drawEditorUI();

	/// <summary> ///
	///		Centers the string in targetBase around a text line with
	///			input margin.
	/// </summary> ///

	std :: string centeredString(const char * targetBase, unsigned margin);
};


#endif
