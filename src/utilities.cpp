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
	while((err = glGetError()) != GL_NO_ERROR)
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


