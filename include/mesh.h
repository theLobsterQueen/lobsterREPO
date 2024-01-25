#ifndef LOBSTER_MESH
#define LOBSTER_MESH 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <vector>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <pipeline.h>

/// <summary> ///
///		This class holds the basic vertex/index/texture and other such attributes
///			so that the object as a whole can be rendered easily by OpenGL.
/// </summary> ///

struct Mesh
{
	std :: vector<GLfloat> vertexPositions;
	unsigned short int vertexCount = 0;
};

// FUNCTION DEFINITIONS //
namespace mesh
{
	/// <summary> ///
	///		This function creates and returns a new mesh component with input vertices.
	/// </summary> ///
	Mesh * createMesh(std :: vector<GLfloat> inputVertices);

	/// <summary> ///
	///		This function draws an input mesh component.
	/// </summary> ///
	void draw(Mesh * inputMesh, Pipeline * pipeline);
};

#endif
