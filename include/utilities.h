#ifndef LOBSTER_UTILS
#define LOBSTER_UTILS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <stdio.h>
#include <iostream>
#include <fstream>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// LOBSTER INCLUDES //
#include <input.h>

// STRUCT PROTOTYPES //
struct InputState;

/// <summary> ///
///		These classes are used to debug GLSL shaders and OpenGL shader program objects.
///			Source: https://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php
/// </summary> ///

void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);
void checkErrors();

/// <summary> ///
///		Functions for reducing the amount of wet code in the input processor
///			on the update function.
/// </summary> ///

void mouseButtonProcess(InputState * inputState, SDL_Event inputEvent, bool setTrue);
void modifierKeyProcess(InputState * inputState, SDL_Event inputEvent, bool setTrue);

/// <summary> ///
///		This function adds an entry to the constants file. If this is the first time
///			calling this function since start up, clears the constant file of all old data.
/// </summary> ///

// STATIC VARIABLE //
static std :: fstream constantFile
	("./shaders/constants.txt", std :: ios_base :: trunc | std :: ios_base :: out);
static bool initialized = false;

template <class T>
void pushConstant(const char * constantName, T constantValue)
{
	if(!constantFile.is_open())
	{
		constantFile.open
		(
			"./../../shaders/constants.txt", 
			std :: ios_base :: trunc | std :: ios_base :: out
		);

		if(!constantFile.is_open())
		{
			std :: cout << "COULD NOT FIND CONSTANTS FILE!" << std :: endl;
			return;
		}
	}
	
	// IF LENGTH OF FILE IS ZERO, ADDS IN OPENING COMMENT //
	if(initialized == false)
	{
		constantFile << std :: endl << "// DEFINES CONSTANTS //" << std :: endl;
		initialized = true;
	}

	// VARIABLE INITIALIZATION //
	std :: string entry = "";
	std :: string exit = "";
	bool failed = true;

	// DISCOVERS TYPE OF INPUT VALUE AND CONFIGURES STRING ENTRY ACCORDINGLY //
	if(typeid(T) == typeid(float))
	{
		entry = "float ";
		exit = "f;";
		failed = false;
	}
	
	if(typeid(T) == typeid(unsigned int))
	{
		entry = "unsigned int ";
		exit = ";";
		failed = false;
	}

	if(typeid(T) == typeid(int))
	{
		entry = "int ";
		exit = ";";
		failed = false;
	}

	if(failed)
	{
		std :: cout << "ERROR! CALLED FUNCTION WITH INVALID TYPE!" << std :: endl;
		return;
	}

	// ADDS TEXT TO FILE //
	constantFile << entry << constantName << " = " << constantValue 
		<< exit << std :: endl;
}

#endif
