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

glm :: mat4x4 transformHandler :: getObjectWorldMatrix(Transform * inputTrans)
{
	// CREATES ROTATION MATRIX //
	glm :: mat4x4 rotMat = transformHandler :: getRotateMatrix(inputTrans);
	glm :: mat4x4 transMat = transformHandler :: getTranslateMatrix(inputTrans);
	glm :: mat4x4 scaleMat = transformHandler :: getScaleMatrix(inputTrans);

	// RETURNS WORLD MATRIX //
	return transMat * rotMat * scaleMat;
}

glm :: mat4x4 transformHandler :: getTranslateMatrix(Transform * inputTrans)
{
	glm :: vec3 data(inputTrans -> position[0], inputTrans -> position[1], inputTrans -> position[2]);
	return glm :: translate(glm :: mat4x4(1.0f), data); 
}

glm :: mat4x4 transformHandler :: getScaleMatrix(Transform * inputTrans)
{
	glm :: vec3 data(inputTrans -> scale[0], inputTrans -> scale[1], inputTrans -> scale[2]);
	return glm :: scale(glm :: mat4x4(1.0f), data); 
}

glm :: mat4x4 transformHandler :: getRotateMatrix(Transform * inputTrans)
{
	glm :: mat4x4 rotMatrix = glm :: mat4x4(1.0f);
	for(char i = 0; i < 2; i++)
	{
		glm :: vec3 axis(0.0f, 0.0f, 0.0f);
		axis[i] = 1.0f;
		rotMatrix = glm :: rotate(rotMatrix, glm :: radians(inputTrans -> rotation[i]), axis);
	}

	return rotMatrix;
}

void transformHandler :: translate
	(Transform * inputTrans, std :: vector<float> deltaVector, bool applyGlobal)
{
	glm :: vec3 delta(deltaVector[0], deltaVector[1], deltaVector[2]);
	if(!applyGlobal)
	{
		glm :: mat4x4 rotMat = getRotateMatrix(inputTrans);
		delta = glm :: vec4(delta.x, delta.y, delta.z, 1.0f) * rotMat;
	}

	for(int i = 0; i < 3; i++)
		inputTrans -> position[i] = inputTrans -> position[i] + delta[i];
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

