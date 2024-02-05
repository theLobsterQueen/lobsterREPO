// INCLUDES DEFINITION FILE //
#include <transform.h>

// FUNCTION IMPLEMENTATIONS //
Transform * transformHandler :: createTransform()
{
	Transform * newTrans = new Transform;
	newTrans -> position = std :: vector<float> { 0.0f, 0.0f, 0.0f };
	newTrans -> rotation = std :: vector<float> { 0.0f, 0.0f, 0.0f };
	newTrans -> scale = std :: vector<float> { 1.0f, 1.0f, 1.0f };

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

LobMatrix transformHandler :: getWorldMat(Transform * inputTrans)
{
	// CREATES ROTATION MATRIX //
	LobMatrix rotMat = 
		math :: rotateMatrix
			(std :: vector<float> { 1, 0, 0 }, inputTrans -> rotation[0]) *
		math :: rotateMatrix
			(std :: vector<float> { 0, 1, 0 }, inputTrans -> rotation[1]) *
		math :: rotateMatrix
			(std :: vector<float> { 0, 0, 1 }, inputTrans -> rotation[2]);

	// CREATES SCALED TRANSLATION MATRIX //
	LobMatrix scaleMat =
	{
		std :: vector<float>
		{
			(inputTrans -> scale[0]), 0.0f, 0.0f, 0.0f,
			0.0f, (inputTrans -> scale[1]), 0.0f, 0.0f,
			0.0f, 0.0f, (inputTrans -> scale[2]), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		},
		4, 4
	};

	LobMatrix transMat =
	{
		std :: vector<float>
		{
			// COLUMN ONE //
			1, 0, 0, inputTrans -> position[0],

			// COLUMN TWO //
			0, 1, 0, inputTrans -> position[1],

			// COLUMN THREE //
			0, 0, 1, -(inputTrans -> position[2]),

			// COLUMN FOUR //
			0, 0, 0, 1
		},
		4, 4 // IS FOUR COLUMNS WIDE AND FOUR ROWS TALL //
	};

	// RETURNS WORLD MATRIX //
	return scaleMat * rotMat * transMat;
}
