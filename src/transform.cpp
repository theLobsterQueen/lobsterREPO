// INCLUDES DEFINITION FILE //
#include <transform.h>

// FUNCTION IMPLEMENTATIONS //
Transform * transformHandler :: createTransform()
{
	Transform * newTrans = new Transform;
	return newTrans;
}

Transform * transformHandler :: createTransform

// PARAMETERS //
(
	std :: vector<float> inputPosition,
	std :: vector<float> inputRotation,
	std :: vector<float> inputScale
)

// FUNCTIONS //
{
	Transform * newTrans = new Transform;
	newTrans -> position = inputPosition;
	newTrans -> rotation = inputRotation;
	newTrans -> scale = inputScale;

	return newTrans;
}

Transform * transformHandler :: duplicateTransform(Transform * base)
{
	Transform * newTransform = transformHandler :: createTransform();
	newTransform -> position = base -> position;
	newTransform -> rotation = base -> rotation;
	newTransform -> scale = base -> scale;
	return newTransform;
}

LobMatrix transformHandler :: getModelWorldMatrix(Transform * inputTrans)
{
	// CREATES ROTATION MATRIX //
	LobMatrix rotMat = transformHandler :: getRotateMatrix(inputTrans);
	LobMatrix transMat = transformHandler :: getTranslateMatrix(inputTrans);
	LobMatrix scaleMat = transformHandler :: getScaleMatrix(inputTrans);

	// RETURNS WORLD MATRIX //
	return scaleMat * rotMat * transMat;
}

LobMatrix transformHandler :: getTranslateMatrix(Transform * inputTrans)
{
	return LobMatrix
	(
		std :: vector<float>
		{
			// COLUMN ONE //
			1, 0, 0, inputTrans -> position[0],

			// COLUMN TWO //
			0, 1, 0, inputTrans -> position[1],

			// COLUMN THREE //
			0, 0, 1, inputTrans -> position[2],

			// COLUMN FOUR //
			0, 0, 0, 1
		},
		4, 4 // IS FOUR COLUMNS WIDE AND FOUR ROWS TALL //
	);
}

LobMatrix transformHandler :: getScaleMatrix(Transform * inputTrans)
{
	return LobMatrix
	(
		std :: vector<float>
		{
			(inputTrans -> scale[0]), 0.0f, 0.0f, 0.0f,
			0.0f, (inputTrans -> scale[1]), 0.0f, 0.0f,
			0.0f, 0.0f, (inputTrans -> scale[2]), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		},
		4, 4
	);
}

LobMatrix transformHandler :: getRotateMatrix(Transform * inputTrans)
{
	return math :: identityMatrix() * 
		math :: rotateMatrix
			(std :: vector<float> { 1.0f, 0.0f, 0.0f }, inputTrans -> rotation[0]) *
		math :: rotateMatrix
			(std :: vector<float> { 0.0f, 1.0f, 0.0f }, inputTrans -> rotation[1]) *
		math :: rotateMatrix
			(std :: vector<float> { 0.0f, 0.0f, 1.0f }, inputTrans -> rotation[2]);
}

void transformHandler :: translate
	(Transform * inputTrans, std :: vector<float> deltaVector)
{
	std :: vector<float> localDelta = 
		math :: vecByMat(deltaVector, transformHandler :: getRotateMatrix(inputTrans));
	for(int i = 0; i < 3; i++)
		inputTrans -> position[i] = inputTrans -> position[i] + localDelta[i];
}

void transformHandler :: rotate
	(Transform * inputTrans, std :: vector<float> deltaVector)
{
	for(int i = 0; i < 3; i++)
		inputTrans -> rotation[i] = inputTrans -> rotation[i] + deltaVector[i];
}

void transformHandler :: scale
	(Transform * inputTrans, std :: vector<float> deltaVector)
{
	for(int i = 0; i < 3; i++)
		inputTrans -> scale[i] = inputTrans -> scale[i] + deltaVector[i];
}

std :: vector<float> transformHandler :: getGlobalPosition(Transform * inputTrans)
{
	Transform * tempTransform = transformHandler :: duplicateTransform(inputTrans);
	LobMatrix invMat = math :: inverse(transformHandler :: getRotateMatrix(inputTrans));
	std :: vector<float> rotVec = { invMat.getPoint(3, 0), invMat.getPoint(3, 1), invMat.getPoint(3, 2) };
	transformHandler :: rotate(tempTransform, rotVec);
	return tempTransform -> position;
}
