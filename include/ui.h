#ifndef LOBSTER_UI_H 
#define LOBSTER_UI_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <filesystem>

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
};


#endif
