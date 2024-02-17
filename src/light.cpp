// INCLUDES DEFINITION FILE //
#include <light.h>

Light * lightHandler :: createLight(std :: vector<float> lightColor)
{
	// VARIABLE INITIALIZATION //
	Light * newLight = new Light;

	// ASSIGNS VARIABLES //
	newLight -> color = lightColor;

	// RETURNS CONSTRUCTED OBJECT //
	return newLight;
}
