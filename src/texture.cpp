// INCLUDES DEFINITION FILE //
#include <texture.h>

// FUNCTION IMPLEMENTATIONS //
Texture * textureHandler :: createTexture(std :: string textureName)
{
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
	return newTex;
}
