#ifndef LOBSTER_PIPELINE
#define LOBSTER_PIPELINE 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <iostream>
#include <fstream>
#include <filesystem>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <utilities.h>
#include <globals.h>
#include <importantConstants.h>
#include <vendorIncludes.h>

/// <summary> ///
///		This struct holds an OpenGL program, and containers for the latest loaded shaders
///			of each programmable type of shader.
/// </summary> ///

struct Pipeline
{
	GLuint program;
	GLuint vertShader; std :: string vertShaderName;
	GLuint fragShader; std :: string fragShaderName;
	bool isCompiled = false;
};

namespace graphicManagement
{
	// PUBLIC METHODS //

	/// <summary> ///
	///		Creates and returns a new, fully initialized pipeline instance.
	/// </summary> ///
	Pipeline * createPipeline();

	/// <summary> ///
	///		Loads a shader at input shader path into input target pipeline, and compiles
	//			it as input shader type. The shader can be printed fully in console
	///			if the caller sets input debug bool as true.
	/// </summary> ///
	void loadShader
	// PARAMETERS //
	(
		Pipeline * targetPipeline, GLenum shaderType, const char * shaderPath, 
		bool debugPrint = false
	);

	/// <summary> ///
	///		Attaches the latest loaded shaders to the render pipeline.
	/// </summary> ///
	void compileProgram(Pipeline * targetPipeline);

	/// <summary> ///
	///		Sets program of input target pipeline as the active program.
	/// </summary> ///
	void usePipeline(Pipeline * targetPipeline);

	/// <summary> ///
	///		This function clears the screen a specific color, and prepares the
	///			engine for another render pass.
	/// </summary> ///
	void beginRenderPass();

	/// <summary> ///
	///		Flips the buffers over using a more engine-friendly function
	///			than the lower level SDL2 function.
	/// </summary> ///
	void present();
	void clearScreen(std :: vector<float> color);

	/// <summary> ///
	///		Prints out the current state of the pipeline.
	/// </summary> ///
	void printPipeline(Pipeline * inputPipeline);
};

#endif
