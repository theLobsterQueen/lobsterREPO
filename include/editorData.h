#ifndef LOBSTER_EDITOR_DATA_H
#define LOBSTER_EDITOR_DATA_H 1

// INCLUDES AND INITIALIZATIONS //

// LOBSTER INCLUDES //
#include <vendorIncludes.h>

/// <summary> ///
///		This struct holds data for the Lobster Editor, such as flags for UI
///			processes or the general theme of the editor.
/// </summary> ///

struct EditorData
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGuiTreeNodeFlags sceneTreeFlags = ImGuiTreeNodeFlags_DefaultOpen;

	/// <summary> ///
	///		This ID represents the entity currently selected within the scene hiearchy.
	/// </summary> ///

	entityID curActiveEntity;
	bool entitySelected = false;
};

#endif
