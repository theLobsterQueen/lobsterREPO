// INCLUDES DEFINITION FILE //
#include <camera.h>

// FUNCTION IMPLEMENTATION //
Camera * cameraHandler :: 
	createCamera(float inputAspect, float inputFOV, Pipeline * inputPipeline)
{
	Camera * newCamera = new Camera;
	newCamera -> aspectRatio = inputAspect;
	newCamera -> FOV = 1 / tan(math :: toRadians(inputFOV / 2));
	newCamera -> curPipelineRef = inputPipeline;

	return newCamera;
}

LobMatrix cameraHandler :: getViewProjMatrix(Camera * camera)
{
	LobMatrix projMatrix = LobMatrix
	(
		std :: vector<float>
		{
			// FIRST COLUMN //
			camera -> FOV / camera -> aspectRatio, 0, 0, 0,

			// SECOND COLUMN //
			0, camera -> FOV, 0, 0,

			// THIRD COLUMN //
			0, 0, camera -> far / (camera -> near - camera -> far), -1,

			// FOURTH COLUMN //
			0, 0, (camera -> far * camera -> near) / 
				(camera -> near - camera -> far), 0
		},
		4, 4
	);

	return projMatrix;
}

LobMatrix cameraHandler :: getWorldViewMatrix(Camera * camera, Transform * camTrans)
{
	// CREATES VIEW MATRIX //
	LobMatrix worldViewMat = LobMatrix
	(
		std :: vector<float>
		{
			// FIRST COLUMN //
			camera -> right[0], camera -> right[1], camera -> right[2], 
				math :: dot(math :: scaleVec(camTrans -> position, -1), camera -> right),

			// SECOND COLUMN //
			camera -> up[0], camera -> up[1], camera -> up[2], 
				math :: dot(math :: scaleVec(camTrans -> position, -1), camera -> up),

			// THIRD COLUMN //
			-(camera -> look[0]), -(camera -> look[1]), -(camera -> look[2]), 
				math :: dot(camTrans -> position, camera -> look),

			// FOURTH COLUMN //
			0, 0, 0, 1
		},
		4, 4
	);

	return worldViewMat;
}
