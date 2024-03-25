// INCLUDES DEFINITION FILE //
#include <camera.h>

// FUNCTION IMPLEMENTATION //
Camera * cameraHandler :: createCamera(float inputAspect, Pipeline * inputPipeline)
{
	Camera * newCamera = new Camera;

	// IF INPUT ASPECT RATIO IS NULL, DETERMINES INPUT ASPECT RATIO OF VIEWPORT //
	if(inputAspect != 0.0f)
		newCamera -> aspectRatio = inputAspect;
	else
		newCamera -> aspectRatio = ((float) (globals :: winWidth)) / ((float) (globals :: winHeight));
	newCamera -> FOV = 70;

	// IF INPUT PIPELINE IS NULLPTR, CREATES AN EMPTY ONE TO BE USED //
	if(inputPipeline == nullptr)
		inputPipeline = graphicManagement :: createPipeline();
	newCamera -> curPipelineRef = inputPipeline;

	return newCamera;
}

void cameraHandler :: zoom(Camera * camera, float zoomDelta)
{
	// (TODO) REDO THIS VALUE: THE LAST IMPLEMENTATION WAS NOT PROPERLY CONFIGURED //
		// IT RESULTED IN MASSIVE PERSPECTIVE SHEARING //
}

glm :: mat4x4 cameraHandler :: getWorldViewMatrix(Camera * camera, Transform * transform)
{
	glm :: vec3 position(transform -> position[0], transform -> position[1], transform -> position[2]);
	return glm :: lookAt
	(
	 	position, position + glm :: vec3
			(glm :: vec4(0.0f, 0.0f, -1.0f, 1.0f) * transformHandler :: getRotateMatrix(transform)),
		glm :: vec3(0.0f, 1.0f, 0.0f)
	);
}

