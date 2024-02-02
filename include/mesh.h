#ifndef LOBSTER_MESH
#define LOBSTER_MESH 1

// INCLUDES AND INITIALIZATIONS //

// STD INCLUDES //
#include <vector>
#include <cmath>

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
	std :: vector<GLfloat> vertexBuffer = { };
	std :: vector<GLfloat> colorBuffer = { };

	unsigned short int vertexCount = 0;
	GLuint VAO = 0;
};

// FUNCTION DEFINITIONS //
namespace meshManagement
{
	/// <summary> ///
	///		This function creates and returns a new mesh component with input vertices,
	///			and assigns it the color of input color struct.
	/// </summary> ///
	Mesh * createMesh
		(std :: vector<GLfloat> inputVertices, std :: vector<GLfloat> inputColor);

	/// <summary> ///
	///		This function draws an input mesh component.
	/// </summary> ///
	void draw(EngineCore * core, Mesh * inputMesh);
};

#endif
