#ifndef LOBSTER_UTILS
#define LOBSTER_UTILS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

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


#endif
