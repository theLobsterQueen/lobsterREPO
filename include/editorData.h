#ifndef LOBSTER_EDITOR_DATA_H
#define LOBSTER_EDITOR_DATA_H 1

// INCLUDES AND INITIALIZATIONS //

// LOBSTER INCLUDES //
#include <vendorIncludes.h>
#include <engineCore.h>

// PROTOTYPE STRUCTS //
struct EngineCore;

/// <summary> ///
///		This struct holds data for the Lobster Editor, such as flags for UI
///			processes or the general theme of the editor.
/// </summary> ///

struct EditorData
{
	/// <summary> ///
	///		These flags define the baseline behavior for different types of UI
	///			objects. 
	/// </summary> ///

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGuiTreeNodeFlags sceneTreeFlags = ImGuiTreeNodeFlags_DefaultOpen;

	/// <summary> ///
	///		This ID represents the entity currently selected within the scene hiearchy.
	/// </summary> ///

	entityID curActiveEntity;
	bool entitySelected = false;

	/// <summary> ///
	///		These values describe the parameters in which the editor frontend is organized.
	/// </summary> ///

	float sidePanelWidth = 0;
	float sidePanelHeight = 0;

	float bottomPanelWidth = 0;
	float bottomPanelHeight = 0;
};

// FUNCTION DEFINITIONS //

namespace editorManagement
{
	/// <summary> ///
	///		This function creates, configures, and returns an instance of an editor data
	///			struct.
	/// </summary> ///

	EditorData * createEditorData(EngineCore * core);
};

#endif
