// INCLUDES DEFINITION FILE //
#include <texture.h>

// FUNCTION IMPLEMENTATIONS //
Texture * textureHandler :: createTexture(std :: string textureName)
{
	// CHECKS TO SEE IF TEXTURE IS ALREADY CACHED //
	if(globals :: textureCache.count(textureName) > 0)
	{
		Texture * cacheTex = globals :: textureCache[textureName];
		Texture * newTex = new Texture;

		newTex -> name = cacheTex -> name;
		newTex -> pixels = cacheTex -> pixels;
		newTex -> width = cacheTex -> width; 
		newTex -> height = cacheTex -> height;
		return newTex;
	}

	// LOADS TEXTURE DATA //
	int width; int height;
	unsigned char * data = stbi_load
	(
		std :: string("./assets/textures/" + textureName).c_str(), 
		&width, &height, 0, 4
	);

	if(data == nullptr)
		data = stbi_load
		(
			std :: string("./../../assets/textures/" + textureName).c_str(), 
			&width, &height, 0, 4
		);

	if(data == nullptr)
	{
		std :: cout << "ERROR! COULD NOT FIND " << textureName << "!" << std :: endl;
		return nullptr;
	}

	// CREATES TEXTURE FROM DATA //
	Texture * newTex = new Texture;

	newTex -> name = textureName;
	newTex -> pixels = data;
	newTex -> width = width; newTex -> height = height;

	// ADDS NEW TEXTURE TO CACHE //
	globals :: textureCache[textureName] = newTex;
	return newTex;
}
