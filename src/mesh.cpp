// INCLUDES DEFINITIONS AND NAMESPACE //
#include <mesh.h>

// FUNCTION IMPLEMENTATIONS //
Mesh * meshHandler :: createMesh

// PARMETERS //
(
	std :: vector<GLfloat> inputVertices, 
	std :: vector<GLuint> inputIndices,
	std :: string meshName
)

// FUNCTION //
{
	// CREATES NEW MESH STRUCT //
	Mesh * newMesh = new Mesh;
	newMesh -> name = meshName;
	newMesh -> texName = std :: string("");

	// LOADS VERTEX DATA INTO CLASS MEMORY //
	newMesh -> vertexData = inputVertices;
	newMesh -> indexData = inputIndices;

	// DERIVES VERTEX COUNT FROM INPUT VERTICES //
	newMesh -> vertexCount = inputVertices.size() / 12;
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
	int vertexSize = 12 * sizeof(float);

	glVertexAttribPointer
	(
		POSITION_VEC, 3, GL_FLOAT, GL_FALSE, 
		vertexSize, 0
	);
	glVertexAttribPointer
	(
		NORMAL_VEC, 3, GL_FLOAT, GL_FALSE,
		vertexSize, (void *) (3 * sizeof(float))
	);
	glVertexAttribPointer
	(
		CORD_VEC, 2, GL_FLOAT, GL_FALSE, 
		vertexSize, (void *) (6 * sizeof(float))
	);
	glVertexAttribPointer
	(
		COLOR_VEC, 4, GL_FLOAT, GL_TRUE,
		vertexSize, (void *) (8 * sizeof(float))
	);


	// ENABLES POSITION/COLOR ATTRIBUTES ON VAO //
	glEnableVertexAttribArray(POSITION_VEC);
	glEnableVertexAttribArray(COLOR_VEC);
	glEnableVertexAttribArray(CORD_VEC);
	glEnableVertexAttribArray(NORMAL_VEC);

	// UNBINDS VECTOR BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return newMesh;
}

void meshHandler :: setTexture(Mesh * inputMesh, Texture * inputTexture)
{
	// SETS MESH'S TEXTURE NAMES //
	inputMesh -> texName = inputTexture -> name;

	// BINDS TEXTURE DATA, IF AVAILABLE //
	glBindTexture(GL_TEXTURE_2D, inputMesh -> textureID);
	glTexImage2D
	(
		GL_TEXTURE_2D, 0, GL_RGBA,
		inputTexture -> width, inputTexture -> height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, inputTexture -> pixels
	);

	// SETS THE TEXTURE TO USE LINEAR INTERPOLATION //
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
	for(int i = 0; i < inputMesh -> vertexData.size(); i += 12)
	{
		std :: vector<float> vertex = 
		{ 
			inputMesh -> vertexData[i],
			inputMesh -> vertexData[i + 1],
			inputMesh -> vertexData[i + 2],

			inputMesh -> vertexData[i + 3],
			inputMesh -> vertexData[i + 4],
			inputMesh -> vertexData[i + 5],

			inputMesh -> vertexData[i + 6],
			inputMesh -> vertexData[i + 7],

			inputMesh -> vertexData[i + 8],
			inputMesh -> vertexData[i + 9],
			inputMesh -> vertexData[i + 10],
			inputMesh -> vertexData[i + 11]
		};

		std :: cout << "VERTEX AT (" << vertex[0] << ", " 
			<< vertex[1] << ", " << vertex[2] << ")" << std :: endl;
		std :: cout << "NORMAL CORDS OF VERTEX ARE (" << vertex[3] << ", " 
			<< vertex[4] << ", " << vertex[5] << ")" << std :: endl;
		std :: cout << "UV CORDS OF VERTEX ARE (" << vertex[6] << ", "
			<< vertex[7] << ")" << std :: endl;
		std :: cout << "COLOR VALUE OF VERTEX IS (" << vertex[8] << ", "
			<< vertex[9] << ", " << vertex[10] << ", " << vertex[11]
			<< ")" << std :: endl;
	}
}

Mesh * meshHandler :: getMeshFromPLY(std :: string inputName)
{
	// VARIABLE INITIALIZATION //
	unsigned int vertexCount;

	std :: vector<float> vertices;
	std :: vector<unsigned int> indices;
	std :: string line; 

	// LOADS FILES //
	std :: ifstream meshFile("assets/models/" + inputName);
	
	// IF COULD NOT LOAD, ATTEMPTS TO LAOD FROM PERSPECTIVE OF EXECUTABLE LIBRARY //
	if(!meshFile.is_open())
		meshFile.open("./../../assets/models/" + inputName);

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
					// READS NON-COLOR COORDINATES //
					if(tokensRead < 8)
						vertices.push_back(std :: stof(data));
					else
						vertices.push_back(std :: stof(data) / 255.0f);
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
	return meshHandler :: createMesh(vertices, indices, inputName);
}
