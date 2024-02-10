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

/// <summary> ///
///		This class holds the basic vertex/index/texture and other such attributes
///			so that the object as a whole can be rendered easily by OpenGL.
/// </summary> ///
struct Mesh
{
	std :: vector<GLfloat> vertexData = { };
	std :: vector<GLuint> indexData = { };
	std :: vector<GLfloat> colorData = { };

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint colorBuffer;

	unsigned short int vertexCount = 0;
	unsigned short int indexCount = 0;
	GLuint VAO = 0;
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
		std :: vector<GLfloat> inputColors
	);

	void drawMesh(Mesh * inputMesh);

	Mesh * getMeshFromPLY(std :: string fileName);

	void printMesh(Mesh * inputMesh);
};

#endif
