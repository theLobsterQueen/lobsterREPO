// INCLUDES DEFINITIONS AND NAMESPACE //
#include <mesh.h>

// FUNCTION IMPLEMENTATIONS //
Mesh * mesh :: createMesh
	(std :: vector<GLfloat> inputVertices, std :: vector<GLfloat> inputColors)
{
	// CREATES NEW MESH STRUCT //
	Mesh * newMesh = new Mesh;

	// ASSURES VALIDITY OF INPUT //
	if(inputVertices.size() % 3 != 0)
	{
		std :: cout << "ERROR: INPUT VERTICES ARE NOT EASILY DIVISBLE BY 3!" 
			<< std :: endl; 
		return newMesh;
	}

	if(inputColors.size() % 4 != 0)
	{
		std :: cout << "ERROR: INPUT VERTICES ARE NOT EASILY DIVISBLE BY 4!" 
			<< std :: endl; 
		return newMesh;
	}

	// LOADS VERTEX DATA INTO CLASS MEMORY //
	newMesh -> vertexBuffer = inputVertices;
	newMesh -> colorBuffer = inputColors;

	// DERIVES VERTEX COUNT FROM INPUT VERTICES //
	newMesh -> vertexCount = inputVertices.size() / 3;
	
	// CREATES A VERTEX BUFFER, ASSINGS ITS VERTEX DATA, AND BINDS TO VERTEX ARRAY //
	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		newMesh -> vertexBuffer.size() * sizeof(GLfloat),
		newMesh -> vertexBuffer.data(),
		GL_STATIC_DRAW
	);

	// CREATES A COLOR BUFFER, ASSIGNS IT COLOR DATA, AND BINDS TO COLOR ARRAY //
	GLuint colorBuffer = 0;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		newMesh -> colorBuffer.size() * sizeof(GLfloat),
		newMesh -> colorBuffer.data(),
		GL_STATIC_DRAW
	);

	// BINDS THE VERTEX ARRAY OBJECT OF THE MESH //
	glGenVertexArrays(1, &(newMesh -> VAO));
	glBindVertexArray(newMesh -> VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(POSITION_VEC, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(COLOR_VEC, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	// ENABLES POSITION/COLOR ATTRIBUTES ON VAO //
	glEnableVertexAttribArray(POSITION_VEC);
	glEnableVertexAttribArray(COLOR_VEC);

	return newMesh;
}

void mesh :: draw(Mesh * inputMesh, Pipeline * pipeline)
{
	// USES THE INPUT PIPELINE //
	graphics :: usePipeline(pipeline);

	// PREPARES TO DRAW WITH INPUT MESH'S VAO //
	glBindVertexArray(inputMesh -> VAO);

	// DRAWS MESH //
	glDrawArrays(GL_TRIANGLES, 0, inputMesh -> vertexCount);
}
