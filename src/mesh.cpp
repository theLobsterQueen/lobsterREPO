// INCLUDES DEFINITIONS AND NAMESPACE //
#include <mesh.h>

// FUNCTION IMPLEMENTATIONS //
Mesh * mesh :: createMesh(std :: vector<GLfloat> inputVertices)
{
	// CREATES NEW MESH STRUCT //
	Mesh * newMesh = new Mesh;

	// ASSURES VALIDITY OF INPUT //
	if(inputVertices.size() % 3 != 0)
	{
		std :: cout << "ERROR: INPUT VERTICES ARE NOT VALID, MUST BE SETS OF "
			<< "THREE VEC3'S PER TRIANGLE!" << std :: endl;
		return newMesh;
	}

	// LOADS VERTEX DATA INTO CLASS MEMORY //
	newMesh -> vertexPositions = inputVertices;

	// DERIVES VERTEX COUNT FROM INPUT VERTICES //
	newMesh -> vertexCount = inputVertices.size() / 3;

	return newMesh;
}

void mesh :: draw(Mesh * inputMesh, Pipeline * pipeline)
{
	// USES THE INPUT PIPELINE //
	pipeline -> use();

	// BINDS THE BUFFER TO THE CONTEXT AND ENSURE IT'S CORRECTLY CONFIGURED //
	glBufferData
	(
		GL_ARRAY_BUFFER, 
		inputMesh -> vertexPositions.size() * sizeof(GLfloat), 
		inputMesh -> vertexPositions.data(), 
		GL_STATIC_DRAW
	);

	// CONFIGURES VERTEX POSITION DATA //
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);			

	// DRAWS MESH //
	glDrawArrays(GL_TRIANGLES, 0, inputMesh -> vertexCount);
}
