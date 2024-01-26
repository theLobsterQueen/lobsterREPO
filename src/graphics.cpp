// INCLUDES DEFINITION //
#include <graphics.h>

Pipeline * graphicManagement :: createPipeline()
{
	Pipeline * newPipeline = new Pipeline; 
	newPipeline -> program = glCreateProgram(); 
	newPipeline -> vertShader = glCreateShader(GL_VERTEX_SHADER);
	newPipeline -> fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	return newPipeline;
}

void graphicManagement :: loadShader
// PARAMETERS //
(
	Pipeline * targetPipeline, GLenum shaderType, const char * shaderPath, 
	bool debugPrint
)

// FUNCTION //
{
	// DETERMINES WHICH SHADER CONTAINER TO ATTACH NEW SHADER TOO //
	GLuint * targetShader;
	switch(shaderType)
	{
		case GL_VERTEX_SHADER :
			targetShader = &(targetPipeline -> vertShader);
		break;

		case GL_FRAGMENT_SHADER :
			targetShader = &(targetPipeline -> fragShader);
		break;

		default :
			std :: cout << "ERROR! INVALID SHADER TYPE WHILE IMPORTING " << shaderPath
				<< std :: endl;
			return;
		break;
	}

	// CREATES NEW SHADER TO ATTACH //
	GLuint newShader = glCreateShader(shaderType);

	// FINDS AND READS SHADER FILE //
	std :: string shaderString, line;
	std :: ifstream shaderFile(shaderPath);
	if(!shaderFile.is_open())
	{
		// ATTEMPTS TO LOAD IT FROM THE PERSEPCTIVE OF AN EXECUTABLE IN DEBUG/RELEASE //
		shaderFile.open(std :: string("../../") + shaderPath);
		if(!shaderFile.is_open())
		{
			std :: cout << "COULD NOT FIND SHADER AT " << shaderPath << "!" << std :: endl;
			return;
		}
	}

	std :: string uniOpCaller("!! UNIFORMS");
	while(std :: getline(shaderFile, line))
	{
		// SEARCHES FOR '!!', WHICH INDICATES CUSTOM OPERATION //
		if(line == uniOpCaller)
		{
			// VARIABLE INITIALIZATION //
			std :: ifstream uniFile("shaders/uniforms.txt");
			if(!uniFile.is_open())
			{
				// ATTEMPTS LOADING FFROM ALTERNATE DIRECTORY //
				uniFile.open("../../shaders/uniforms.txt");
				if(!uniFile.is_open())
				{
					std :: cout << "ERROR! COULD NOT FIND UNIFORMS FILE." << std :: endl;
					return;
				}
			}

			// COPIES DATA INTO SHADER //
			std :: string tempLine;
			while(std :: getline(uniFile, tempLine))
				shaderString += tempLine + "\n";

			continue;
		}

		// IF NOT PRODUCT OF CUSTOM OPERATION, COPIES RAW TEXT INTO SHADER //
		shaderString += line + "\n";
	}

	if(debugPrint)
		std :: cout << std :: endl << shaderString << std :: endl;

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

	// ATTACHES NEW SHADER TO TARGET CONTAINER //
	(*targetShader) = newShader;

	// CLOSES SHADER FILE //
	shaderFile.close();
}

void graphicManagement :: compileProgram(Pipeline * targetPipeline)
{
	// VARIABLE INITIALIZATION //
	GLuint prog = targetPipeline -> program;
	GLuint vert = targetPipeline -> vertShader;
	GLuint frag = targetPipeline -> fragShader;
	bool failed = false;

	// ENSURES THAT EACH SHADER IS LOADED //
	if(vert == 0)
	{
		std :: cout << "ATTEMPTING TO COMPILE A RENDER PROGRAM WHILE "
			<< "VERTEX SHADER IS NOT LOADED!" << std :: endl;
		failed = true;
	}

	if(frag == 0)
	{
		std :: cout << "ATTEMPTING TO COMPILE A RENDER PROGRAM WHILE "
			<< "FRAGMENT SHADER IS NOT LOADED!" << std :: endl;
		failed = true;
	}

	if(failed)
		return;

	// ATTACHES SHADERS AND LINKS PROGRAM //
	glAttachShader(prog, vert); glAttachShader(prog, frag);
	glLinkProgram(prog);

	// CHECKS FOR ERRORS IN LINKING //
	GLint operationSuccess = GL_FALSE;
	glGetProgramiv(prog, GL_LINK_STATUS, &operationSuccess);
	if(operationSuccess == GL_FALSE)
	{
		std :: cout << "ERROR WHILE LINKING PROGRAM: ";
		printProgramLog(prog);
		std :: cout << std :: endl;
		return;
	}
}

void graphicManagement :: usePipeline(Pipeline * targetPipeline)
{ 
	glUseProgram(targetPipeline -> program); 
}

void graphicManagement :: beginRenderPass(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void graphicManagement :: present(EngineCore * core)
	{ SDL_GL_SwapWindow(core -> winRef); }
