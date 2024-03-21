#ifndef LOBSTER_MESH
#define LOBSTER_MESH 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <graphics.h>
#include <importantConstants.h>
#include <scene.h>
#include <math.h>
#include <texture.h>

/// <summary> ///
///		This class holds the basic vertex/index/texture and other such attributes
///			so that the object as a whole can be rendered easily by OpenGL.
/// </summary> ///
struct Mesh
{
	std :: string name;
	std :: string texName;

	std :: vector<GLfloat> vertexData = { };
	std :: vector<GLuint> indexData = { };

	GLuint vertexBuffer;
	GLuint indexBuffer;

	unsigned short int vertexCount = 0;
	unsigned short int indexCount = 0;
	GLuint VAO = 0;

	GLuint textureID = 0;	
	SDL_Surface * texSurf;
};

// FUNCTION DEFINITIONS //
namespace meshHandler
{
	/// <summary> ///
	///		This function creates and returns a new mesh component with input vertices,
	///			and assigns it the color of input color struct.
	/// </summary> ///
	Mesh * createMesh
	(
		std :: vector<GLfloat> inputVertices, 
		std :: vector<GLuint> inputIndices,
		std :: string meshName = "mesh"
	);

	void drawMesh(Mesh * inputMesh);

	Mesh * getMeshFromPLY(std :: string fileName, bool debugPrint = false);

	void printMesh(Mesh * inputMesh);

	// <summary> ///
	///		This function loads a texture image from input file and assigns its
	///			pixel data to the input mesh struct.
	/// </summary> ///

	void setTexture(Mesh * inputMesh, Texture * inputTexture);
};

#endif
