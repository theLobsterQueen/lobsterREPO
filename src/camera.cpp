// INCLUDES DEFINITION FILE //
#include <camera.h>

// FUNCTION IMPLEMENTATION //
Camera * cameraHandler :: 
	createCamera(float inputAspect, float inputFOV, Pipeline * inputPipeline)
{
	Camera * newCamera = new Camera;
	float FOV = math :: toRadians(inputFOV / 2);

	newCamera -> aspectRatio = inputAspect;
	newCamera -> hozFOV = 1 / (tan(inputAspect * FOV));
	newCamera -> verFOV = 1 / (tan(FOV));
	newCamera -> curPipelineRef = inputPipeline;

	return newCamera;
}

LobMatrix cameraHandler :: getViewMatrix(Camera * camera, Transform * camTrans)
{
	// CREATES VIEW MATRIX //
	LobMatrix viewMat = LobMatrix
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

	return viewMat;
}
