#ifndef LOB_TEX_H
#define LOB_TEX_H 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <string>
#include <iostream>

// VENDOR INCLUDES //
#include <stb_image.h>

/// <summary> ///
///		This struct  defines the dimensions of the image, and holds a reference
///			to the beginning of the texture's pixel data in memory.
/// </summary> ///

struct Texture
{
	unsigned char * pixels;
	unsigned width; unsigned height;
	std :: string name;
};

// FUNCTION DEFINITIONS //
namespace textureHandler
{
	Texture * createTexture(std :: string textureName);
};

#endif
