// INCLUDES DEFINITION FILE //
#include <camera.h>

// FUNCTION IMPLEMENTATION //
Camera * cameraHandler :: createCamera(float inputAspect, Pipeline * inputPipeline)
{
	Camera * newCamera = new Camera;
	newCamera -> aspectRatio = inputAspect;
	newCamera -> FOV = 1;

	// IF INPUT PIPELINE IS NULLPTR, CREATES AN EMPTY ONE TO BE USED //
	if(inputPipeline == nullptr)
		inputPipeline = graphicManagement :: createPipeline();
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
	LobMatrix rotMat = transformHandler :: getRotateMatrix(camTrans);
	std :: vector<float> look = math :: vecByMat
		(std :: vector<float> { 0.0f, 0.0f, 1.0f, 0.0f }, rotMat);
	look.pop_back();
	std :: vector<float> up = math :: vecByMat
		(std :: vector<float> { 0.0f, 1.0f, 0.0f, 0.0f }, rotMat);
	up.pop_back();
	std :: vector<float> right = math :: cross(look, up);
	up = math :: cross(right, look);

	LobMatrix worldViewMat = LobMatrix
	(
		std :: vector<float>
		{
			// FIRST COLUMN //
			right[0], right[1], right[2], math :: dot
				(math :: scaleVec(camTrans -> position, -1), right),

			// SECOND COLUMN //
			up[0], up[1], up[2], math :: dot
				(math :: scaleVec(camTrans -> position, -1), up),

			// THIRD COLUMN //
			-look[0], -look[1], -look[2], math :: dot
				(camTrans -> position, look),

			// FOURTH COLUMN //
			0, 0, 0, 1
		},
		4, 4
	);

	return worldViewMat;
}

void cameraHandler :: zoom(Camera * camera, float zoomDelta)
{
	// (TODO) REDO THIS VALUE: THE LAST IMPLEMENTATION WAS NO PROPERLY CONFIGURED //
		// IT RESULTED IN MASSIVE PERSPECTIVE SHEARING //
}
