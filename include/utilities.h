#ifndef LOBSTER_UTILS
#define LOBSTER_UTILS 1

// INCLUDES AND INITIALIZATIONS //

// STANDARD INCLUDES //
#include <stdio.h>
#include <iostream>

// SDL/GL INCLUDES //
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

/// <summary> ///
///		These classes are used to debug GLSL shaders and OpenGL shader program objects.
///			Source: https://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php
/// </summary> ///

void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);
void checkErrors();


#endif
