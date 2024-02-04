// INCLUDES DEFINITIONS AND NAMESPACE //
#include <mesh.h>

// FUNCTION IMPLEMENTATIONS //
Mesh * meshManagement :: createMesh

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
	newMesh -> vertexBuffer = inputVertices;
	newMesh -> indexBuffer = inputIndices;
	newMesh -> colorBuffer = inputColors;

	// DERIVES VERTEX COUNT FROM INPUT VERTICES //
	newMesh -> vertexCount = inputVertices.size() / 3;
	newMesh -> indexCount = inputIndices.size();

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

	// CREATES A VERTEX BUFFER, ASSINGS ITS VERTEX DATA, AND BINDS TO VERTEX ARRAY //
	GLuint indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		newMesh -> indexBuffer.size() * sizeof(GLuint),
		newMesh -> indexBuffer.data(),
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

	// UNBINDS VECTOR BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return newMesh;
}

void meshManagement :: draw(EngineCore * core, Mesh * inputMesh)
{
	// USES THE INPUT PIPELINE //
	Scene * targetScene = core -> curSceneRef;

	// CREATES ROTATION MATRIX //
	LobMatrix rotMat = 
		math :: rotateMatrix(std :: vector<float> { 1, 0, 0 }, core -> tempRot[0]) *
		math :: rotateMatrix(std :: vector<float> { 0, 1, 0 }, core -> tempRot[1]) *
		math :: rotateMatrix(std :: vector<float> { 0, 0, 1 }, core -> tempRot[2]);

	// CREATES SCALED TRANSLATION MATRIX //
	LobMatrix scaleMat =
	{
		std :: vector<float>
		{
			(core -> tempScale), 0.0f, 0.0f, 0.0f,
			0.0f, (core -> tempScale), 0.0f, 0.0f,
			0.0f, 0.0f, (core -> tempScale), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		},
		4, 4
	};

	LobMatrix transMat =
	{
		std :: vector<float>
		{
			// COLUMN ONE //
			1, 0, 0, targetScene -> viewRef -> playerPos[0],

			// COLUMN TWO //
			0, 1, 0, targetScene -> viewRef -> playerPos[1],

			// COLUMN THREE //
			0, 0, 1, -(targetScene -> viewRef -> playerPos[2]),

			// COLUMN FOUR //
			0, 0, 0, 1
		},
		4, 4 // IS FOUR COLUMNS WIDE AND FOUR ROWS TALL //
	};

	LobMatrix worldMat = scaleMat * rotMat * transMat;

	// SETS OBJECT WORLD MATRIX //
	glUniformMatrix4fv
	(
		UNI_WORLD_MATRIX,  // WHAT UNIFORM IT'S SETTING //
		1, GL_FALSE, worldMat.getData()
	);

	// PREPARES TO DRAW WITH INPUT MESH'S VAO //
	glBindVertexArray(inputMesh -> VAO);

	// DRAWS MESH //
	glDrawElements
	(
		GL_TRIANGLES, 
		inputMesh -> indexCount, 
		GL_UNSIGNED_INT,
		inputMesh -> indexBuffer.data()
	);
}
