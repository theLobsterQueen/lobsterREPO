// INCLUDES AND INITIALIZATIONS //
#include <utilities.h>

void printProgramLog(GLuint program)
{
    //Make sure name is shader
    if( glIsProgram( program ) )
    {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = new char[ maxLength ];
        
        //Get info log
        glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }
        
        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a program\n", program );
    }
}

void printShaderLog(GLuint shader)
{
    //Make sure name is shader
    if( glIsShader( shader ) )
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = new char[ maxLength ];
        
        //Get info log
        glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a shader\n", shader );
    }
}

void checkErrors()
{
	GLenum err;
	while(err = glGetError() != GL_NO_ERROR)
	{
		switch(err)
		{
			case GL_INVALID_ENUM :
				std :: cout << "ERROR: GL_INVALID_ENUM!" << std :: endl;
			break;

			case GL_INVALID_VALUE :
				std :: cout << "ERROR: GL_INVALID_VALUE!" << std :: endl;
			break;

			case GL_INVALID_OPERATION :
				std :: cout << "ERROR: GL_INVALID_OPERATION!" << std :: endl;
			break;

			case GL_STACK_OVERFLOW :
				std :: cout << "ERROR: GL_STACK_OVERFLOW!" << std :: endl;
			break;

			case GL_STACK_UNDERFLOW :
				std :: cout << "ERROR: GL_STACK_UNDERFLOW!" << std :: endl;
			break;

			case GL_OUT_OF_MEMORY :
				std :: cout << "ERROR: GL_OUT_OF_MEMORY!" << std :: endl;
			break;

			case GL_INVALID_FRAMEBUFFER_OPERATION :
				std :: cout << "ERROR: GL_INVALID_FRAMEBUFFER_OPERATION!" << std :: endl;
			break;

			case GL_CONTEXT_LOST :
				std :: cout << "ERROR: GL_CONTEXT_LOST!" << std :: endl;
			break;
		}
	}
}


void mouseButtonProcess(InputState * inputState, SDL_Event inputEvent, bool setTrue)
{
	static int mouseX; static int mouseY;
	auto key = inputEvent.button.button;
	switch(key)
	{
		case SDL_BUTTON_LEFT :
			inputState -> lmb = setTrue;
		break;

		case SDL_BUTTON_MIDDLE :
			inputState -> mmb = setTrue;
		break;

		case SDL_BUTTON_RIGHT :
			inputState -> rmb = setTrue;
			if(setTrue)
			{
				SDL_GetMouseState(&mouseX, &mouseY);
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}

			else
			{
				SDL_WarpMouseInWindow(NULL, mouseX, mouseY);
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		break;
	}
}

void modifierKeyProcess(InputState * inputState, SDL_Event inputEvent, bool setTrue)
{
	auto scanKey = inputEvent.key.keysym.scancode;
	if(scanKey == SDL_SCANCODE_LSHIFT)
		inputState -> shiftPressed = setTrue;

	else if(scanKey == SDL_SCANCODE_LALT)
		inputState -> altPressed = setTrue;

	else if(scanKey == SDL_SCANCODE_LCTRL)
		inputState -> cntrlPressed = setTrue;
}

std :: string compToString(componentID compID)
{
	switch(compID)
	{
		case TRANS_COMP_ID :
			return std :: string("Transform");
		break;

		case MESH_COMP_ID :
			return std :: string("Mesh");
		break;

		case LIGHT_COMP_ID :
			return std :: string("Light");
		break;

		case CAMERA_COMP_ID :
			return std :: string("Camera");
		break;

		case SCRIPT_COMP_ID :
			return std :: string("Script");
		break;
	}
	return std :: string("NULL");
}

compPtr constructComp(componentID compID)
{
	switch(compID)
	{
		case TRANS_COMP_ID :
			return (compPtr) transformHandler :: createTransform();
		break;

		case MESH_COMP_ID :
			std :: cout << "INVALID CONSTRUCT COMP! CANNOT CONSTRUCT A COMP OF " << 
				compToString(MESH_COMP_ID) << std :: endl;
			return nullptr;
		break;

		case LIGHT_COMP_ID :
			return (compPtr) lightHandler :: createLight();
		break;

		case CAMERA_COMP_ID :
			return (compPtr) cameraHandler :: createCamera();
		break;

		case SCRIPT_COMP_ID :
			std :: cout << "INVALID CONSTRUCT COMP! CANNOT CONSTRUCT A COMP OF " << 
				compToString(SCRIPT_COMP_ID) << std :: endl;
			return nullptr;
		break;
	}
	return nullptr;
}
