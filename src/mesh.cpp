// INCLUDES DEFINITIONS AND NAMESPACE //
#include <mesh.h>

// FUNCTION IMPLEMENTATIONS //
Mesh * meshHandler :: createMesh

// PARMETERS //
(
	std :: vector<GLfloat> inputVertices, 
	std :: vector<GLuint> inputIndices, 
	std :: vector<GLfloat> inputColors
)

// FUNCTION //
{
	// CREATES NEW MESH STRUCT //
	Mesh * newMesh = new Mesh;

	// LOADS VERTEX DATA INTO CLASS MEMORY //
	newMesh -> vertexData = inputVertices;
	newMesh -> indexData = inputIndices;
	newMesh -> colorData = inputColors;

	// DERIVES VERTEX COUNT FROM INPUT VERTICES //
	newMesh -> vertexCount = inputVertices.size() / 3;
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

	// CREATES A COLOR BUFFER, ASSIGNS IT COLOR DATA, AND BINDS TO COLOR ARRAY //
	glGenBuffers(1, &(newMesh -> colorBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, newMesh -> colorBuffer);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		newMesh -> colorData.size() * sizeof(GLfloat),
		newMesh -> colorData.data(),
		GL_STATIC_DRAW
	);

	// BINDS THE VERTEX ARRAY OBJECT OF THE MESH //
	glGenVertexArrays(1, &(newMesh -> VAO));
	glBindVertexArray(newMesh -> VAO);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh -> vertexBuffer);
	glVertexAttribPointer(POSITION_VEC, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh -> colorBuffer);
	glVertexAttribPointer(COLOR_VEC, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	// ENABLES POSITION/COLOR ATTRIBUTES ON VAO //
	glEnableVertexAttribArray(POSITION_VEC);
	glEnableVertexAttribArray(COLOR_VEC);

	// UNBINDS VECTOR BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return newMesh;
}

Mesh * meshHandler :: copyMesh(Mesh * inputMesh)
{
	return meshHandler :: createMesh
		(inputMesh -> vertexData, inputMesh -> indexData, inputMesh -> colorData);
}

void meshHandler :: drawMesh(Mesh * inputMesh)
{
	// PREPARES TO DRAW WITH INPUT MESH'S VAO //
	glBindVertexArray(inputMesh -> VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inputMesh -> indexBuffer);

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

		std :: vector<float> color = 
		{ 
			inputMesh -> colorData[i],
			inputMesh -> colorData[i + 1],
			inputMesh -> colorData[i + 2]
		};

		std :: cout << "COLOR OF VERTEX IS (" << color[0] << ", " 
			<< color[1] << ", " << color[2] << ")" << std :: endl;
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
	std :: vector<float> colors;
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
					if(tokensRead < 3)
						vertices.push_back(std :: stof(data));

					// READS COLOR COORDINATES //
					else
						colors.push_back(std :: stoi(data) / 255.0f);
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
	return meshHandler :: createMesh(vertices, indices, colors);
}
