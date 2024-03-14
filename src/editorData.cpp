// DEFINITION FILE INCLUDE //
#include <editorData.h>

// FUNCTION IMPLEMENTATIONS //
EditorData * editorManagement :: createEditorData(EngineCore * core)
{
	EditorData * temp = new EditorData;

	// DETERMINES THE DIMENSIONS OF SIDE PANELS //
	temp -> sidePanelWidth = (core -> winWidth) * 0.2f;
	temp -> sidePanelHeight = (core -> winHeight) * 0.8f;

	// DETERMINES THE DIMENSIONS OF BOTTOM PANELS //
	temp -> bottomPanelWidth = (core -> winWidth);
	temp -> bottomPanelHeight = (core -> winHeight) * 0.2f;

	// RETURNS THE NEWLY CONFIGURED EDITOR DATA STRUCT //
	return temp;
}
