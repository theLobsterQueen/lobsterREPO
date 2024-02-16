// INCLUDES DEFINITIONS AND NAMESPACE //
#include <mesh.h>

// FUNCTION IMPLEMENTATIONS //
Mesh * meshHandler :: createMesh

// PARMETERS //
(
	std :: vector<GLfloat> inputVertices, 
	std :: vector<GLuint> inputIndices,
	unsigned char elementsPerVertex
)

// FUNCTION //
{
	// CREATES NEW MESH STRUCT //
	Mesh * newMesh = new Mesh;

	// LOADS VERTEX DATA INTO CLASS MEMORY //
	newMesh -> vertexData = inputVertices;
	newMesh -> indexData = inputIndices;

	// DERIVES VERTEX COUNT FROM INPUT VERTICES //
	newMesh -> vertexCount = inputVertices.size() / elementsPerVertex;
	newMesh -> indexCount = inputIndices.size();

	// CREATES A VERTEX BUFFER, ASSINGS ITS VERTEX DATA, AND BINDS TO VERTEX ARRAY //
	glGenBuffers(1, &(newMesh -> vertexBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, newMesh -> vertexBuffer);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		newMesh -> vertexData.size() * sizeof(GLfloat),
		newMesh -> vertexData.data(),
		GL_STATIC_DRAW
	);

	// CREATES A VERTEX BUFFER, ASSINGS ITS VERTEX DATA, AND BINDS TO VERTEX ARRAY //
	glGenBuffers(1, &(newMesh -> indexBuffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh -> indexBuffer);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		newMesh -> indexData.size() * sizeof(GLuint),
		newMesh -> indexData.data(),
		GL_STATIC_DRAW
	);
	
	// BINDS THE VERTEX ARRAY OBJECT OF THE MESH //
	glGenVertexArrays(1, &(newMesh -> VAO));
	glBindVertexArray(newMesh -> VAO);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh -> vertexBuffer);
	glVertexAttribPointer
	(
		POSITION_VEC, 3, GL_FLOAT, GL_FALSE, 
		elementsPerVertex * sizeof(float), 0
	);
	glVertexAttribPointer
	(
		CORD_VEC, 2, GL_FLOAT, GL_FALSE, 
		elementsPerVertex * sizeof(float), (void *) (3 * sizeof(float))
	);

	// ENABLES POSITION/COLOR ATTRIBUTES ON VAO //
	glEnableVertexAttribArray(POSITION_VEC);
	glEnableVertexAttribArray(CORD_VEC);

	// UNBINDS VECTOR BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return newMesh;
}

void meshHandler :: loadTexture(Mesh * inputMesh, std :: string texturePath)
{
	inputMesh -> texSurf = IMG_Load
		(std :: string("assets/textures/" + texturePath).c_str());
	if(inputMesh -> texSurf == nullptr)
		inputMesh -> texSurf = IMG_Load
			(std :: string("./../../assets/texutres/" + texturePath).c_str());
	
	if(inputMesh -> texSurf == nullptr)
	{
		std :: cout << "ERROR! COULD NOT FIND " << texturePath << "!" << std :: endl;
		return;
	}

	// VERTICALLY FLIPS THE PIXEL DATA //

	// CODE TAKEN FROM USER vvanpelt ON //
	// https://stackoverflow.com/questions/65815332/ flipping-a-surface-vertically-in-sdl2

    int pitch = inputMesh -> texSurf -> pitch; // row size
    char* temp = new char[pitch]; // intermediate buffer
    char* pixels = (char*) inputMesh -> texSurf -> pixels;
    
    for(int i = 0; i < inputMesh -> texSurf -> h / 2; ++i) {
        // get pointers to the two rows to swap
        char* row1 = pixels + i * pitch;
        char* row2 = pixels + (inputMesh -> texSurf -> h - i - 1) * pitch;
        
        // swap rows
        memcpy(temp, row1, pitch);
        memcpy(row1, row2, pitch);
        memcpy(row2, temp, pitch);
    }
    
    delete[] temp;

	// IF SUCCESSFULLY LOADED, CONFIGURES TEXTURE DATA //
	glGenTextures(1, &(inputMesh -> textureID));
	GLenum textureFormat = 0;

	switch(inputMesh -> texSurf -> format -> format)
	{
		case SDL_PIXELFORMAT_RGBA32 :
			textureFormat = GL_RGBA;
		break;

		case SDL_PIXELFORMAT_RGB444 :
		case SDL_PIXELFORMAT_RGB24  :
			textureFormat = GL_RGB;
		break;

		default :
			std :: cout << "UNSUPPORTED TEXTURE FORMAT! RETURNING!"
				" IMAGE FORMAT IS " << 
				SDL_GetPixelFormatName(inputMesh -> texSurf -> format -> format)
				<< "." << std :: endl;
			return;
		break;
	}

	// BINDS TEXTURE DATA, IF AVAILABLE //
	glBindTexture(GL_TEXTURE_2D, inputMesh -> textureID);
	glTexImage2D
	(
		GL_TEXTURE_2D, 0, GL_RGBA8,
		inputMesh -> texSurf -> w, inputMesh -> texSurf -> h, 
		0, textureFormat, GL_UNSIGNED_BYTE, inputMesh -> texSurf -> pixels
	);

	// SETS THE TEXTURE TO USE LINEARL INTERPOLATION //
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// CLAMPS THE TEXTURE IF IT EXTENDS PAST THE U/V BOUNDARIES //
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// UNBINDS THE TEXTURE OBJECT //
	glBindTexture(GL_TEXTURE_2D, 0);
}

void meshHandler :: drawMesh(Mesh * inputMesh)
{
	// PREPARES TO DRAW WITH INPUT MESH'S VAO //
	glBindVertexArray(inputMesh -> VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inputMesh -> indexBuffer);

	// BINDS TEXTURE FOR MESH //
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, inputMesh -> textureID);
	glUniform1i(UNI_2D_SAMPLER, 0);

	// DRAWS MESH //
	glDrawElements
	(
		GL_TRIANGLES, 
		inputMesh -> indexCount, 
		GL_UNSIGNED_INT,
		nullptr
	);
}

void meshHandler :: printMesh(Mesh * inputMesh)
{
	for(int i = 0; i < inputMesh -> vertexData.size(); i += 3)
	{
		std :: vector<float> vertex = 
		{ 
			inputMesh -> vertexData[i],
			inputMesh -> vertexData[i + 1],
			inputMesh -> vertexData[i + 2]
		};

		std :: cout << "VERTEX AT (" << vertex[0] << ", " 
			<< vertex[1] << ", " << vertex[2] << ")" << std :: endl;
		std :: cout << "TEX CORDS OF VERTEX ARE (" << vertex[3] << ", " 
			<< vertex[4] << ")" << std :: endl;
	}

	std :: cout << "INDICES ARE : " << std :: endl;
	for(int i = 0; i < inputMesh -> indexData.size(); i++)
		std :: cout << "\t" << inputMesh -> indexData[i] << std :: endl;
}

Mesh * meshHandler :: getMeshFromPLY(std :: string inputName)
{
	// VARIABLE INITIALIZATION //
	unsigned int vertexCount;

	std :: vector<float> vertices;
	std :: vector<unsigned int> indices;
	std :: string line; 

	// LOADS FILES //
	std :: ifstream meshFile(inputName);
	
	// IF COULD NOT LOAD, ATTEMPTS TO LAOD FROM PERSPECTIVE OF EXECUTABLE LIBRARY //
	if(!meshFile.is_open())
		meshFile.open("./../../" + inputName);

	// IF STILL NOT OPEN, RAISES ERROR //
	if(!meshFile.is_open())
	{
		std :: cout << "ERROR! COULD NOT LOAD " << inputName << "!" << std :: endl;
		return nullptr;
	}

	// IF FILE WAS SUCCESSFULLY OPENED, FILLES VERTEX/INDEX/COLOR DATA //
	bool foundPly = false; bool isASCII = false;
	bool readData = false;
	while(std :: getline(meshFile, line))
	{
		if(readData == false)
		{
			// ATTEMPTS TO FIND "ply" KEYWORD WHICH MARKS FILE AS .ply FILE //
			if(line.find(std :: string("ply")) != std :: string :: npos)
				foundPly = true;

			// ATTEMPTS TO FIND "format ascii 1.0," WHICH IS THE ONLY VERSION OF //
				// .ply FILE THAT THIS FUNCTION ACCEPTS //
			if(line.find(std :: string("format ascii 1.0")) != std :: string :: npos)
				isASCII = true;

			// DETERMINES HOW MANY VERTICES ARE IN THE MESH // 
			if(line.find(std :: string("element vertex")) != std :: string :: npos)
			{
				std :: stringstream lineStream(line);
				std :: string data;
				int index = 0;
				while(std :: getline(lineStream, data, ' '))
				{
					if(index == 2)
						vertexCount = std :: stoi(data);
					index++;
				}
			}

			// IF FIND "end_header" MARKER, BEGINS TO READ DATA //
			if(line.find(std :: string("end_header")) != std :: string :: npos)
			{
				// IF THE CORRECT VERSION IDENTIFIER OR THE "ply" IDENTIFIER WERE //
					// NOT FOUND, RAISES ERROR //
				if(!(foundPly && isASCII))
				{
					std :: cout << "ERROR! INPUT FILE " << inputName 
						<< " DID NOT HAVE PROPER " << "PLY/VERSION IDENTIFIER!" 
						<< std :: endl;
					return nullptr;
				}

				readData = true;
				continue;
			}
		}	

		else
		{
			// VARIABLE INITIALIZATION //
			std :: stringstream lineStream(line);
			std :: string data;
			unsigned char tokensRead = 0;
			bool readingVertex = false;

			// DETERMINES WHAT TYPE OF DATA IS ON THE CURRENT LINE //
			if(vertexCount > 0)
			{
				vertexCount--;
				readingVertex = true;
			}

			// READS THE LINE //
			while(std :: getline(lineStream, data, ' '))
			{
				// READS VERTEX/COLOR DATA //
				if(readingVertex)
				{
					// READS X/Y/Z COORDINATES //
					vertices.push_back(std :: stof(data));
				}

				// READS INDEX DATA //
				else
				{
					// ENSURES THAT FUNCTION IS ACTUALLY READING A LIST OF TRIANGLE //
						// ELEMENTS. IF NOT, RAISES ERROR. //
					if(tokensRead == 0)
					{
						if(std :: stoi(data) != 3)
						{
							std :: cout << "ERROR! AN ELEMENT IN .ply FILE IS NOT LENGTH "
								<< "THREE! (REMEMBER TO TRIANGULATE YOUR MODELS BEFORE "
								<< "LOADING THEM!)" << std :: endl;
							return nullptr;
						}
					}

					// READS INDEX DATA //
					else
						indices.push_back((unsigned int) (std :: stoi(data)));
				}

				// INCREMENTS DATA POINTER TO NEXT TOKEN IN LINE //
				tokensRead++;
			}
		}
	}

	std :: cout << "LOADED " << inputName << " SUCCESSFULY!" << std :: endl;
	return meshHandler :: createMesh(vertices, indices, vertexCount);
}
