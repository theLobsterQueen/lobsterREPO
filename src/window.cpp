// INCLUDES DEFINITION AND USES DEFINITION NAMESPACE //
#include <window.h>

// IMPLEMENT FUNCTIONS //

void windowManagement :: changeTitle(SDL_Window * window, std :: string windowName)
	{ SDL_SetWindowTitle(window, windowName.c_str()); }
