// INCLUDES DEFINITION AND USES DEFINITION NAMESPACE //
#include <window.h>

// IMPLEMENT FUNCTIONS //

void windowManagement :: changeTitle(SDL_Window * window, std :: string windowName)
	{ SDL_SetWindowTitle(window, windowName.c_str()); }

void windowManagement :: changeSize(unsigned x, unsigned y)
{ 
	globals :: winWidth = x;
	globals :: winHeight = y;
	SDL_SetWindowSize(globals :: winRef, x, y); 
	determineUIBounds();

	if(!(editorGlobals :: optionsRef -> hideUIWhilePlaying) || !(globals :: isPlaying))
		glViewport
		(
			editorGlobals :: sidePanelWidth,
			editorGlobals :: bottomPanelHeight,
			globals :: winWidth - (editorGlobals :: sidePanelWidth * 2),
			globals :: winHeight - editorGlobals :: bottomPanelHeight
		);
	else
		glViewport(0, 0, globals :: winWidth, globals :: winHeight);
}
