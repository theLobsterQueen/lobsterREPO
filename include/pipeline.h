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

// INTERNAL INCLUDES //
#include <utilities.h>

/// <summary> ///
///		This class is a basic render pipeline to be used by the engine. It holds a reference
///			to all types of shader objects in an OpenGL pipeline, a program for
///			those shaders to be compiled to, and the functions to make that process
///			intuitive and effective.
/// </summary> ///

class Pipeline
{
	public :
		// PUBLIC METHODS //
		void init()
			{ program = glCreateProgram(); }

		void loadShader(GLenum shaderType, const char * shaderPath, bool debugPrint = false)
		{
			// ASSERT THAT SHADER TYPE IS EITHER FRAGMENT OR VERTEX SHADER //
			if(!(shaderType == GL_FRAGMENT_SHADER || shaderType == GL_VERTEX_SHADER))
			{
				std :: cout << "ERROR: INVALID ENUM IN loadShader FUNCTION CALL!"
					<< std :: endl;
				return;
			}
			GLuint newShader = glCreateShader(shaderType);

			// FINDS AND READS SHADER FILE //
			std :: string shaderString, line;
			std :: ifstream shaderFile(shaderPath);
			while(std :: getline(shaderFile, line))
			{
				if(debugPrint)
					std :: cout << line << std :: endl;
				shaderString += line + "\n";
			}

			const char * source[] = { shaderString.c_str() };
			glShaderSource(newShader, 1, source, NULL);

			// ATTEMPTS TO COMPILE NEW SHADER //
			glCompileShader(newShader);

			// CHECKS FOR ERRORS //
			GLint operationSuccess = GL_FALSE;
			glGetShaderiv(newShader, GL_COMPILE_STATUS, &operationSuccess);
			if(operationSuccess == GL_FALSE)
			{
				std :: cout << "ERROR WHILE COMPILING " << shaderPath << ": ";
				printShaderLog(newShader);
				std :: cout << std :: endl;
				return;
			}

			// IF ERROR-FREE, ATTACHES SHADER TO PROGRAM //
			glAttachShader(program, newShader);

			// CLOSES SHADER FILE //
			shaderFile.close();
		}

		/// <summary> ///
		///		Attempts to link the program together using the latest loaded shaders.
		/// </summary> ///

		void linkProgram()
		{
			// ATTACHES SHADERS AND LINKS PROGRAM //
			glLinkProgram(program);

			// CHECKS FOR ERRORS IN LINKING //
			GLint operationSuccess = GL_FALSE;
			glGetProgramiv(program, GL_LINK_STATUS, &operationSuccess);
			if(operationSuccess == GL_FALSE)
			{
				std :: cout << "ERROR WHILE LINKING PROGRAM: ";
				printProgramLog(program);
				std :: cout << std :: endl;
				return;
			}
		}

		/// <summary> ///
		///		Sets this classes' program as the active program.
		/// </summary> ///
		void use()
			{ glUseProgram(program); }

	private :
		// PRIVATE ATTRIBUTES //
		GLuint program;
		GLuint vertShader;
		GLuint fragShader;
};

#endif
