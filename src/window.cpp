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
}
