// INCLUDES DEFINITION AND USES DEFINITION NAMESPACE //
#include <graphics.h>

// DEFINES FUNCTIONS //
void graphics :: changeTitle(EngineCore * core, std :: string windowName)
	{ SDL_SetWindowTitle(core -> winRef, windowName.c_str()); }

void graphics :: beginRenderPass(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void graphics :: present(EngineCore * core)
{	 SDL_GL_SwapWindow(core -> winRef); }
